#include "eyeworldtab.h"

EyeWorldTab::EyeWorldTab(QWidget *parent, MainWindow* mW) : QWidget(parent)
{
    //Layout
    layout = new QGridLayout(this);
    imgLblEye = new QLabel("EYE",this);
    imgLblWorld = new QLabel("WORLD",this);
    eyeFpsLabel = new QLabel("FPS (Eye) :",this);
    worldFpsLabel = new QLabel("FPS (World) :",this);
    eyeLatencyLabel = new QLabel("Eye Latency : ", this);
    worldLatencyLabel = new QLabel("World Latency : ", this);
    button_method = new QPushButton("using Ellipse, click to switch to hough circle", this);
    button_interpol = new QPushButton("PPV => Bilinéaire", this);

    QSpacerItem *lastColSpacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem *lastRowSpacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);

    stepsCombo.addItem("Original");
    stepsCombo.addItem("Threshold");
    stepsCombo.addItem("Opening");
    stepsCombo.addItem("Closing");

    slider_threshold = new QSlider(Qt::Horizontal, this);
    slider_threshold->setMinimum(0);
    slider_threshold->setMaximum(125);
    slider_threshold->setValue(50);

    slider_ROI = new QSlider(Qt::Horizontal, this);
    slider_ROI->setMinimum(1);
    slider_ROI->setMaximum(30);
    slider_ROI->setValue(10);

    //Placement in Layout
    layout->addWidget(eyeFpsLabel       ,FPS_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(worldFpsLabel     ,FPS_ROW, IMAGE_WORLD_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(eyeLatencyLabel   ,LATENCY_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(worldLatencyLabel ,LATENCY_ROW, IMAGE_WORLD_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(imgLblEye         ,IMAGES_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(imgLblWorld       ,IMAGES_ROW, IMAGE_WORLD_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(button_method     ,BUTTON_METHOD_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, BUTTON_COLUMN_SPAN);
    layout->addWidget(slider_threshold  ,SLIDER_PARAM_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, SLIDER_COLUMN_SPAN);
    layout->addWidget(slider_ROI        ,SLIDER_PARAM_ROW, IMAGE_WORLD_COLUMN, NORMAL_ROW_SPAN, SLIDER_COLUMN_SPAN);
    layout->addWidget(&stepsCombo       ,COMBOBOX_INTERMEDIATE_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(button_interpol   ,COMBOBOX_INTERMEDIATE_ROW, IMAGE_WORLD_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);

    layout->addItem(lastColSpacer       ,BUTTON_METHOD_ROW, SPACER_COLUMN, SPACER_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addItem(lastRowSpacer       ,SPACER_ROW ,IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, SPACER_COLUMN_SPAN);

    connect(button_method, SIGNAL (clicked()), this, SLOT (switchPupilMethodButton()));
    connect(button_interpol, SIGNAL(clicked()), this, SLOT (switchInterpolMethodButton()));
    mainWindowPtr = mW;

    //generate the gray LUT;
    gray_LUT.create(1, 256, CV_8U);
    gray_level_LUT_pointer = gray_LUT.ptr();
    for (int i = 0; i < 256; i++) {
            gray_level_LUT_pointer[i] = grayLevelsTable[i];
    }
}

void EyeWorldTab::processFrameEye()
{
    startProcessEye = std::chrono::system_clock::now();
    if(frameBufferCam0.size() >= 2)
        imgEye = frameBufferCam0.back();
    if(imgEye.empty()) return;
    int comboBoxIndex = stepsCombo.currentIndex();

    //Crop EyeCam image according to calibration settings
    cropRegion(&imgEye, &imgEyeCropped, mainWindowPtr->calibrationPosX, mainWindowPtr->calibrationPosY, mainWindowPtr->roiSize, mainWindowPtr->roiSize, false);
    //Resize to constant resolution
    cv::resize(imgEyeCropped, imgEyeResized, cv::Size(CAMERA_RESOLUTION, CAMERA_RESOLUTION), 0, 0, cv::INTER_LINEAR);

    (pupilMethod) ? applyEllipseMethod(&imgEyeResized, slider_threshold->value(), posX, posY, comboBoxIndex) : applyHoughMethod(&imgEyeResized, posX, posY) ;

    if(posX >= 0 && posX < CAMERA_RESOLUTION && posY >= 0 && posY < CAMERA_RESOLUTION ) {
        mainWindowPtr->laser_pos_control.send_pos(posX, posY);
        mainWindowPtr->laser_pos_control.laser.on();
        cv::circle(imgEyeResized, cv::Point(posX, posY), 6, (comboBoxIndex == 0) ? cv::Scalar(0, 255, 0) : cv::Scalar(180, 180, 180), -1);
    }
    else
        mainWindowPtr->laser_pos_control.laser.off();

    cv::cvtColor(imgEyeResized, imgEyeToShow, cv::COLOR_BGR2RGB);
    QImage qimgEye(reinterpret_cast<uchar*>(imgEyeToShow.data), imgEyeToShow.cols, imgEyeToShow.rows, imgEyeToShow.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));
    endProcessEye = std::chrono::system_clock::now();
    double msTimeEye = std::chrono::duration<double> ((endProcessEye - startProcessEye)*1000).count();

    //std::cout<< eyeCamLatency.count() <<std::endl;
    if (msTimeEye != 0){
        eyeFpsLabel->setText(QString("FPS (Eye) : %2").arg(round((1/msTimeEye)*1000)));
        eyeLatencyLabel->setText(QString("Latency :%1").arg(round(eyeCamLatency.count())));
    }
}

void EyeWorldTab::processFrameWorld()
{
    startProcessWorld = std::chrono::system_clock::now();
    //used for low pass filter on ROI
    static int previousPosX = posX;
    static int previousPosY = posY;

    if(frameBufferCam1.size() >= 2)
        imgWorld = frameBufferCam1.back();
    if(imgWorld.empty()) return;

    if(posX >= 0 && posX < CAMERA_RESOLUTION && posY >= 0 && posY < CAMERA_RESOLUTION )
    {
        cv::Mat img2World = imgWorld;

        
        int posXWorld;
        int posYWorld;
        if(abs(posX-previousPosX) > LOWPASS_CUTOFF_VALUE || abs(posY-previousPosY) > LOWPASS_CUTOFF_VALUE)
        {
            //Adjust pupil position to worldCam resolution
            posXWorld = posX * imgWorld.cols / CAMERA_RESOLUTION;
            posYWorld = posY * imgWorld.rows / CAMERA_RESOLUTION;
            previousPosX = posX;
            previousPosY = posY;
        }
        else 
        {
            //Adjust pupil position to worldCam resolution
            posXWorld = previousPosX * imgWorld.cols / CAMERA_RESOLUTION;
            posYWorld = previousPosY * imgWorld.rows / CAMERA_RESOLUTION;
        }

        int roi_width = ROI_COL * slider_ROI->value() / 10;
        int roi_height = ROI_LINES * slider_ROI->value() / 10;

        if(imgWorld.channels() <= 1){
            (RECTSHOW) ? cropRegion(&imgWorld, &imgWorldCropped, posXWorld, posYWorld, roi_height, roi_width, true) : cropRegion(&imgWorld, &imgWorldCropped, posXWorld, posYWorld, roi_height, roi_width, false);
        }
        else{
            cv::cvtColor(imgWorld,img2World,cv::COLOR_RGB2GRAY);
            (RECTSHOW) ? cropRegion(&imgWorld, &imgWorldCropped, posXWorld, posYWorld, roi_height, roi_width, true) : cropRegion(&imgWorld, &imgWorldCropped, posXWorld, posYWorld, roi_height, roi_width, false);
        }

        traitementWorld(&imgWorldCropped, gray_LUT);
        drawWorl2img(&imgWorld, &imgWorldCropped, posXWorld, posYWorld, roi_height, roi_width, interpolMethod);
    }

    cv::cvtColor(imgWorld,imgWorldToShow,cv::COLOR_BGR2RGB);
    QImage qimgWorld(reinterpret_cast<uchar*>(imgWorldToShow.data), imgWorldToShow.cols, imgWorldToShow.rows, imgWorldToShow.step, QImage::Format_RGB888);
    imgLblWorld->setPixmap(QPixmap::fromImage(qimgWorld));

    endProcessWorld = std::chrono::system_clock::now();
    double msTimeWorld = std::chrono::duration<double> ((endProcessWorld - startProcessWorld) * 1000 ).count();
    if (msTimeWorld != 0){
        worldFpsLabel->setText(QString("FPS (World) : %1").arg(round((1/msTimeWorld)*1000)));
        worldLatencyLabel->setText(QString("Latency :%1").arg(round(worldCamLatency.count())));
    }
}


void EyeWorldTab::switchPupilMethodButton()
{
    pupilMethod = !pupilMethod; //change stage of pupil detection
    if(VERBOSE)
        std::cout << ((pupilMethod) ? "Change pupil detection method to Ellipse" : "Change pupil detection method to hough circle")<< std::endl;
    button_method->setText((pupilMethod)?"using Ellipse, click to switch to hough circle":"using Hough cricle, click to switch to Ellipse");

    if(!pupilMethod){
        stepsCombo.setEnabled(false);
        stepsCombo.setCurrentIndex(0);
        slider_threshold->setEnabled(false);
        slider_ROI->setEnabled(false);
    }
    else{
        stepsCombo.setEnabled(true);
        slider_threshold->setEnabled(true);
        slider_ROI->setEnabled(true);

    }
}

void EyeWorldTab::switchInterpolMethodButton(){
    //std::cout << "yo yo yo"<<std::endl;
    interpolMethod++;
    if(interpolMethod > 2)
        interpolMethod = 0;
    //interpolMethod = (interpolMethod <= 2) ? 0 : interpolMethod + 1;
    if (interpolMethod == 0)
        button_interpol->setText("PPV => Bilinéaire");
    else if (interpolMethod == 1)
        button_interpol->setText("Bilinéaire => Bicubique");
    else if (interpolMethod == 2)
        button_interpol->setText("Bicubique => PPV");
}

