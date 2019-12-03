#include "eyeworldtab.h"
#include <string>


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

    slider_threshold = new QSlider(Qt::Horizontal, this);
    slider_threshold->setMinimum(0);
    slider_threshold->setMaximum(125);
    slider_threshold->setValue(25);

    slider_ROI = new QSlider(Qt::Horizontal, this);
    slider_ROI->setMinimum(1);
    slider_ROI->setMaximum(30);
    slider_ROI->setValue(10);

    //Placement in Layout
    layout->addWidget(eyeFpsLabel       ,FPS_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(worldFpsLabel     ,FPS_ROW, IMAGE_WORLD_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(imgLblEye         ,IMAGES_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(imgLblWorld       ,IMAGES_ROW, IMAGE_WORLD_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addWidget(button_method     ,BUTTON_METHOD_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, BUTTON_COLUMN_SPAN);
    layout->addWidget(slider_threshold  ,SLIDER_PARAM_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, SLIDER_COLUMN_SPAN);
    layout->addWidget(slider_ROI        ,SLIDER_PARAM_ROW, IMAGE_WORLD_COLUMN, NORMAL_ROW_SPAN, SLIDER_COLUMN_SPAN);
    layout->addWidget(&stepsCombo       ,COMBOBOX_INTERMEDIATE_ROW, IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, NORMAL_COLUMN_SPAN);

    layout->addItem(lastColSpacer       ,BUTTON_METHOD_ROW, SPACER_COLUMN, SPACER_ROW_SPAN, NORMAL_COLUMN_SPAN);
    layout->addItem(lastRowSpacer       ,SPACER_ROW ,IMAGE_EYE_COLUMN, NORMAL_ROW_SPAN, SPACER_COLUMN_SPAN);

    connect(button_method, SIGNAL (clicked()), this, SLOT (switchPupilMethodButton()));
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
    //start = std::chrono::system_clock::now();
    imgEye = *(mainWindowPtr->cameras->readImgCam(0));
    if(imgEye.empty()) return;

    int comboBoxIndex = stepsCombo.currentIndex();

    //Crop EyeCam image according to calibration settings
    cropRegion(&imgEye, &imgEye, mainWindowPtr->calibrationPosX, mainWindowPtr->calibrationPosY, mainWindowPtr->roiSize, mainWindowPtr->roiSize, false);
    //Resize to constant resolution
    cv::resize(imgEye, imgEye, cv::Size(CAMERA_RESOLUTION, CAMERA_RESOLUTION), 0, 0, cv::INTER_LINEAR);

    (pupilMethod) ? applyEllipseMethod(&imgEye, slider_threshold->value(), posX, posY, comboBoxIndex) : applyHoughMethod(&imgEye, posX, posY) ;

    if(posX >= 0 && posX < CAMERA_RESOLUTION && posY >= 0 && posY < CAMERA_RESOLUTION ) {
        mainWindowPtr->laser_pos_control.send_pos(posX, posY);
        mainWindowPtr->laser_pos_control.laser.on();
        if(comboBoxIndex == 0) {
            cv::circle(imgEye, cv::Point(posX, posY), 6, cv::Scalar(0, 255, 0), -1);
        }
        else {
            cv::circle(imgEye, cv::Point(posX, posY), 6, cv::Scalar(180, 180, 180), -1);
        }


    }
    else
        mainWindowPtr->laser_pos_control.laser.off();

    cv::cvtColor(imgEye, imgEye, cv::COLOR_BGR2RGB);
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));
    //end = std::chrono::system_clock::now();
    //std::chrono::duration<double> elapsed_seconds = end-start;
    //std::cout << elapsed_seconds.count() << std::endl;

}

void EyeWorldTab::processFrameWorld()
{

    static int imgSwitchCounter = 0;
    static std::string imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/12.png";
    switch (imgSwitchCounter) {

        case(SECONDS_SWITCH):
            imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/1.png";
        break;

        case(SECONDS_SWITCH*2):
            imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/2.png";
        break;

        case(SECONDS_SWITCH*3):
            imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/4.png";
        break;

        case(SECONDS_SWITCH*4):
            imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/5.png";
        break;

        case(SECONDS_SWITCH*5):
            imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/6.png";
        break;

        case(SECONDS_SWITCH*6):
            imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/7.png";
        break;

        case(SECONDS_SWITCH*7):
            imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/8.png";
        break;

        case(SECONDS_SWITCH*8):
            imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/9.png";
        break;

        case(SECONDS_SWITCH*9):
            imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/10.png";
        break;

        case(SECONDS_SWITCH*10):
            imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/11.png";
        break;

        case(SECONDS_SWITCH*11):
            imagePath = "C:/Users/houma/Downloads/ImagesResized/ImagesResized/12.png";
            imgSwitchCounter = 0;
        break;

        default:
        break;

    }
    imgSwitchCounter += 1;
    imgWorld = cv::imread(imagePath, cv::IMREAD_COLOR);
    //imgWorld = *(mainWindowPtr->cameras)->readImgCam(1);


    if(imgWorld.empty()) return;


    if(posX >= 0 && posX < CAMERA_RESOLUTION && posY >= 0 && posY < CAMERA_RESOLUTION )
    {
        cv::Mat img2World = imgWorld;

        //Adjust pupil position to worldCam resolution
        int posXWorld = posX * imgWorld.cols / CAMERA_RESOLUTION;
        int posYWorld = posY * imgWorld.rows / CAMERA_RESOLUTION;

        int roi_width = ROI_COL * slider_ROI->value() / 10;
        int roi_height = ROI_LINES * slider_ROI->value() / 10;

        if(imgWorld.channels() <= 1){
            (RECTSHOW) ? cropRegion(&imgWorld, &img2World, posXWorld, posYWorld, roi_height, roi_width, true) : cropRegion(&imgWorld, &img2World, posXWorld, posYWorld, roi_height, roi_width, false);
        }
        else{
            cv::cvtColor(imgWorld,img2World,cv::COLOR_RGB2GRAY);
            (RECTSHOW) ? cropRegion(&imgWorld, &img2World, posXWorld, posYWorld, roi_height, roi_width, true) : cropRegion(&imgWorld, &img2World, posXWorld, posYWorld, roi_height, roi_width, false);
        }

        traitementWorld(&img2World, gray_LUT);
        drawWorl2img(&imgWorld, &img2World, posXWorld, posYWorld, roi_height, roi_width);
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
        slider_threshold->setEnabled(false);
        slider_ROI->setEnabled(false);
    }
    else{
        stepsCombo.setEnabled(true);
        slider_threshold->setEnabled(true);
        slider_ROI->setEnabled(true);

    }
}


