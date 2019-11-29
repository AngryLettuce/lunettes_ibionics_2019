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
    connect(&stepsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(comboboxItemChanged(int)));
    mainWindowPtr = mW;
}

void EyeWorldTab::processFrameEye()
{
    int comboBoxIndex = stepsCombo.currentIndex();
    imgEye = *(mainWindowPtr->cameras->readImgCam(0));
    //Crop and resize EyeCam image according to calibration settings
    cropRegion(&imgEye, &imgEye, mainWindowPtr->calibrationPosX, mainWindowPtr->calibrationPosY, mainWindowPtr->roiSize, mainWindowPtr->roiSize, false);
    cv::resize(imgEye, imgEye, cv::Size(CAMERA_RESOLUTION-1, CAMERA_RESOLUTION-1), 0, 0, cv::INTER_LINEAR);

    if(imgEye.channels() <= 1){
        // if the signature of the functions changes and return a cv::Point, we could make it in one line
        (pupilMethod) ? applyEllipseMethod(&imgEye, slider->value(), posX, posY, comboBoxIndex) : applyHoughMethod(&imgEye, posX, posY) ;
        cv::circle(imgEye, cv::Point(posX, posY),7, cv::Scalar(255, 0, 0), -1);
    }
    else{
        cv::Mat img2Eye;
        cv::cvtColor(imgEye, img2Eye, cv::COLOR_RGB2GRAY);
        (pupilMethod) ? applyEllipseMethod(&img2Eye, slider->value(), posX, posY, comboBoxIndex) : applyHoughMethod(&img2Eye, posX, posY) ;
        cv::circle(imgEye, cv::Point(posX, posY),7, cv::Scalar(255, 0, 0), -1);
        cv::cvtColor(imgEye, imgEye, cv::COLOR_BGR2RGB);
    }
	
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));   
    
    //Adjust laser position 
	//TODO Hot Fix
/*
    if (posX < 0 || posY < 0)
    {
        posX = 0;
        posY = 0;
    }
    else
    {
        posX = posX*199/660;
        posY = posY*199/490;
        
        if(posX >= 199)
            posX = 199;
        if(posY >= 199)
            posY = 199;
    }
*/

    std::cout<<"PosX: "<<posX<<"PosY: "<<posY<<std::endl;
    
    mainWindowPtr->laser_pos_control.send_pos(posX, posY);
}

void EyeWorldTab::processFrameWorld()
{
    imgWorld = *(mainWindowPtr->cameras)->readImgCam(1);
    cv::Mat img2World = imgWorld;
	if(imgWorld.empty()) return;
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


