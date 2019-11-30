#include "eyeworldtab.h"

EyeWorldTab::EyeWorldTab(QWidget *parent, MainWindow* mW) : QWidget(parent)
{
    //Layout
    layout = new QGridLayout(this);
    imgLblEye = new QLabel("EYE",this);
    imgLblWorld = new QLabel("WORLD",this);
    button = new QPushButton("using Ellipse, click to switch to hough circle", this);

    stepsCombo.addItem("Original");
    stepsCombo.addItem("Threshold");
    stepsCombo.addItem("Closing");
    stepsCombo.addItem("Opening");

    slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum(255);
    slider->setValue(25);

    //Placement in Layout
    layout->addWidget(imgLblEye,2,0,1,1);
    layout->addWidget(imgLblWorld,2,1,1,1);
    layout->addWidget(button,0,0,1,2);
    layout->addWidget(slider,1,0,1,2);
    layout->addWidget(&stepsCombo,3,0,1,1);

    connect(button, SIGNAL (clicked()), this, SLOT (switchPupilMethodButton()));
    mainWindowPtr = mW;
}

void EyeWorldTab::processFrameEye()
{
    imgEye = *(mainWindowPtr->cameras->readImgCam(0));
    if(imgEye.empty()) return;
    
    int comboBoxIndex = stepsCombo.currentIndex();

    //Crop EyeCam image according to calibration settings
    cropRegion(&imgEye, &imgEye, mainWindowPtr->calibrationPosX, mainWindowPtr->calibrationPosY, mainWindowPtr->roiSize, mainWindowPtr->roiSize, false);
    //Resize to constant resolution
    cv::resize(imgEye, imgEye, cv::Size(CAMERA_RESOLUTION, CAMERA_RESOLUTION), 0, 0, cv::INTER_LINEAR);
    
    (pupilMethod) ? applyEllipseMethod(&imgEye, slider->value(), posX, posY, comboBoxIndex) : applyHoughMethod(&imgEye, posX, posY) ;
    if(posX >= 0 && posX < CAMERA_RESOLUTION && posY >= 0 && posY < CAMERA_RESOLUTION ) {
        cv::circle(imgEye, cv::Point(posX, posY), 7, cv::Scalar(180, 180, 180), -1);
        mainWindowPtr->laser_pos_control.send_pos(posX, posY);
    }

    cv::cvtColor(imgEye, imgEye, cv::COLOR_BGR2RGB);
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));   
    
    //std::cout<<"PosX: "<<posX<<" PosY: "<<posY<<std::endl;
}

void EyeWorldTab::processFrameWorld()
{
    imgWorld = *(mainWindowPtr->cameras)->readImgCam(1);    
	if(imgWorld.empty()) return;
    
    cv::Mat img2World = imgWorld;
    
    //std::cout<<"PosX: "<<posX<<" PosY: "<<posY<<std::endl;
    if(imgWorld.channels() <= 1){
        (RECTSHOW) ? cropRegion(&imgWorld, &img2World, posX, posY, 160, 180, true) : cropRegion(&imgWorld, &img2World, posX, posY, 160, 180, false);
    }
    else{
        cv::cvtColor(imgWorld,img2World,cv::COLOR_RGB2GRAY);
        (RECTSHOW) ? cropRegion(&imgWorld, &img2World, posX, posY, 160, 180, true) : cropRegion(&imgWorld, &img2World, posX, posY, 160, 180, false);
        cv::cvtColor(imgWorld,imgWorld,cv::COLOR_BGR2RGB);
    }

    QImage qimgWorld(reinterpret_cast<uchar*>(imgWorld.data), imgWorld.cols, imgWorld.rows, imgWorld.step, QImage::Format_RGB888);
    imgLblWorld->setPixmap(QPixmap::fromImage(qimgWorld));
}


void EyeWorldTab::switchPupilMethodButton()
{   
    pupilMethod = !pupilMethod; //change stage of pupil detection
    if(VERBOSE)
        std::cout << ((pupilMethod) ? "Change pupil detection method to Ellipse" : "Change pupil detection method to hough circle")<< std::endl;
    button->setText((pupilMethod)?"using Ellipse, click to switch to hough circle":"using Hough cricle, click to switch to Ellipse");
}


