#include "eyeworldtab.h"

EyeWorldTab::EyeWorldTab(QWidget *parent, MainWindow* mW) : QWidget(parent)
{
    //Layout
    layout = new QGridLayout(this);
    imgLblEye = new QLabel("EYE",this);
    imgLblWorld = new QLabel("WORLD",this);
    eyeFpsLabel = new QLabel("FPS (Eye) :",this);
    worldFpsLabel = new QLabel("FPS (World) :",this);
    button_method = new QPushButton("using Ellipse, click to switch to hough circle", this);
    QSpacerItem *lastColSpacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem *lastRowSpacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);


    stepsCombo.addItem("Original");
    stepsCombo.addItem("Threshold");
    stepsCombo.addItem("Opening");
    stepsCombo.addItem("Closing");

    slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum(255);
    slider->setValue(25);

    //Placement in Layout
    layout->addWidget(eyeFpsLabel   ,FPS_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(worldFpsLabel ,FPS_ROW, IMAGE_WORLD_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(imgLblEye     ,IMAGES_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(imgLblWorld   ,IMAGES_ROW, IMAGE_WORLD_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(button_method ,BUTTON_METHOD_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, BUTTON_COLUMN_SPAN);
    layout->addWidget(slider        ,SLIDER_PARAM_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, SLIDER_COLUMN_SPAN);
    layout->addWidget(&stepsCombo   ,COMBOBOX_INTERMEDIATE_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);

    layout->addItem(lastColSpacer   ,BUTTON_METHOD_ROW, SPACER_COLUMN, SPACER_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addItem(lastRowSpacer   ,SPACER_ROW ,IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, SPACER_COLUMN_SPAN);

    connect(button_method, SIGNAL (clicked()), this, SLOT (switchPupilMethodButton()));
    mainWindowPtr = mW;
}

void EyeWorldTab::processFrameEye()
{

    startEvent = std::chrono::system_clock::now();
    imgEye = mainWindowPtr->cameras->frameBufferCam0.back();
    //imgEye = *(mainWindowPtr->cameras->readImgCam(0));
    endReadImg = std::chrono::system_clock::now();
    if(imgEye.empty()) return;

    //imgEye = mainWindowPtr->cameras->frameBufferEyeProceed.back();

    int comboBoxIndex = stepsCombo.currentIndex();

    //Crop EyeCam image according to calibration settings
    startImgProc = std::chrono::system_clock::now();
    cropRegion(&imgEye, &imgEye, mainWindowPtr->calibrationPosX, mainWindowPtr->calibrationPosY, mainWindowPtr->roiSize, mainWindowPtr->roiSize, false);
    endImgProc = std::chrono::system_clock::now();
    //Resize to constant resolution
    cv::resize(imgEye, imgEye, cv::Size(CAMERA_RESOLUTION, CAMERA_RESOLUTION), 0, 0, cv::INTER_LINEAR);
    endResize = std::chrono::system_clock::now();

    (pupilMethod) ? applyEllipseMethod(&imgEye, slider->value(), posX, posY, comboBoxIndex) : applyHoughMethod(&imgEye, posX, posY) ;
    endPupilMethod  = std::chrono::system_clock::now();



    if(posX >= 0 && posX < CAMERA_RESOLUTION && posY >= 0 && posY < CAMERA_RESOLUTION ) {
        mainWindowPtr->laser_pos_control.laser.on();
        cv::circle(imgEye, cv::Point(posX, posY), 7, cv::Scalar(180, 180, 180), -1);
        mainWindowPtr->laser_pos_control.send_pos(posX, posY);
    }
    else
        mainWindowPtr->laser_pos_control.laser.off();

    cv::cvtColor(imgEye, imgEye, cv::COLOR_BGR2RGB);
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));

    endEvent = std::chrono::system_clock::now();
    //----------------------------------------------------------Profiling---------------------------------------------------------------------------------

//    std::cout << "Read Img Time :" <<   (std::chrono::duration<double>(endReadImg-startEvent)).count()*1000 << std::endl;
//    std::cout << "Crop Time : " <<      (std::chrono::duration<double>(endImgProc-startImgProc)).count()*1000 << std::endl;
//    std::cout << "Resize Time : " <<    (std::chrono::duration<double>(endResize-endImgProc)).count()*1000 << std::endl;
//    std::cout << "Method Time : " << (std::chrono::duration<double>(endPupilMethod-endResize)).count()*1000 << std::endl;
//    std::cout << "All Time : " << (std::chrono::duration<double>(endEvent-startEvent)).count()*1000 << std::endl;


}

void EyeWorldTab::processFrameWorld()
{
    imgWorld = mainWindowPtr->cameras->frameBufferCam1.back();
    //imgWorld = mainWindowPtr->cameras->frameBufferWorldProceed.back();
    //imgWorld = *(mainWindowPtr->cameras)->readImgCam(1);
    if(imgWorld.empty()) return;

    if(posX >= 0 && posX < CAMERA_RESOLUTION && posY >= 0 && posY < CAMERA_RESOLUTION )
    {
        cv::Mat img2World = imgWorld;

        //Adjust pupil position to worldCam resolution
        int posXWorld = posX * imgWorld.cols / CAMERA_RESOLUTION;
        int posYWorld = posY * imgWorld.rows / CAMERA_RESOLUTION;

        if(imgWorld.channels() <= 1){
            (RECTSHOW) ? cropRegion(&imgWorld, &img2World, posXWorld, posYWorld, 160, 180, true) : cropRegion(&imgWorld, &img2World, posXWorld, posYWorld, 160, 180, false);
        }
        else{
            cv::cvtColor(imgWorld,img2World,cv::COLOR_RGB2GRAY);
            (RECTSHOW) ? cropRegion(&imgWorld, &img2World, posXWorld, posYWorld, 160, 180, true) : cropRegion(&imgWorld, &img2World, posXWorld, posYWorld, 160, 180, false);
        }

        cv::Mat gray_LUT(1, 256, CV_8U);
        uchar*p = gray_LUT.ptr();
        for (int i = 0; i < 256; i++) {
                p[i] = grayLevelsTable[i];
        }

        traitementWorld(&img2World,gray_LUT);
        drawWorl2img(&imgWorld, &img2World,posXWorld,posYWorld);
        //cv::imshow("test",imgWorld);
    }

    cv::cvtColor(imgWorld,imgWorld,cv::COLOR_BGR2RGB);
    QImage qimgWorld(reinterpret_cast<uchar*>(imgWorld.data), imgWorld.cols, imgWorld.rows, imgWorld.step, QImage::Format_RGB888);
    imgLblWorld->setPixmap(QPixmap::fromImage(qimgWorld));

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
        slider->setEnabled(false);

    }
    else{
        stepsCombo.setEnabled(true);
        slider->setEnabled(true);

    }
}


