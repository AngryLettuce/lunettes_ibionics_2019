#include "eyeworldtab.h"
#include <QCoreApplication>
#include "mainwindow.h"


EyeWorldTab::EyeWorldTab(QWidget *parent, MainWindow* mW) : QWidget(parent)
{
    
    //Layout
    layout = new QGridLayout(this);
    imgLblEye = new QLabel("EYE",this);
    imgLblWorld = new QLabel("WORLD",this);
    button = new QPushButton("using Ellipse, click to switch to hough circle", this);

    //Placement in Layout
    layout->addWidget(imgLblEye,1,0,1,1);
    layout->addWidget(imgLblWorld,1,1,1,1);
    layout->addWidget(button,0,0,1,2);

    connect(button, SIGNAL (clicked()), this, SLOT (switchPupilMethodButton()));
    mainWindowPtr = mW;
    
    //laser_pos_control = nlaser_pos_control();
}

void EyeWorldTab::processFrameEye()
{
    (mainWindowPtr->camEye).read(imgEye);
    if(imgEye.empty()) return;

    cv::Mat img2Eye;
    cv::cvtColor(imgEye,img2Eye,cv::COLOR_RGB2GRAY);

    if(pupilMethod)
        applyEllipseMethod(&img2Eye, posX, posY);
    else
        applyHoughMethod(img2Eye,posX,posY);

    //Adjust laser position 
    posX = posX*199/640;
    posY = posY*199/480;
    laser_pos_control.send_pos(posX, posY);
    
    //Add point to EyeCam
    cv::Point centre = cv::Point(posX,posY);
    cv::circle(imgEye, centre,7, cv::Scalar(255, 0, 0), -1);

    cv::cvtColor(imgEye,imgEye,cv::COLOR_BGR2RGB);
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));   
}

void EyeWorldTab::processFrameWorld()
{
    (mainWindowPtr->camWorld).read(imgWorld);
    if(imgWorld.empty()) return;

    cv::Mat img2World = imgWorld;
    cv::cvtColor(imgWorld,img2World,cv::COLOR_RGB2GRAY);

    if(RECTSHOW)
        cropRegion(imgWorld, &img2World, posX, posY, 160, 180); //show only crop
    else
    {
        cropRegionShow(&imgWorld, &img2World, posX, posY, 160, 180); //show all with rectancle
        //cv::cvtColor(imgWorld,imgWorld,cv::COLOR_BGR2RGB);
        //QImage qimg(reinterpret_cast<uchar*>(img.data), img.cols, img.rows, img.step, QImage::Format_RGB888);
        //img2World = imgWorld.clone();
    }

    cv::cvtColor(imgWorld,imgWorld,cv::COLOR_BGR2RGB);
    QImage qimgWorld(reinterpret_cast<uchar*>(imgWorld.data), imgWorld.cols, imgWorld.rows, imgWorld.step, QImage::Format_RGB888);
    imgLblWorld->setPixmap(QPixmap::fromImage(qimgWorld));
}


void EyeWorldTab::switchPupilMethodButton()
{   
    pupilMethod = !pupilMethod; //change stage of pupil detection
    if(VERBOSE)
    {
        if(pupilMethod)
            std::cout << "Change pupil detection method to Ellipse" << std::endl;
        else
            std::cout << "Change pupil detection method to hough circle" << std::endl;
    }

    if(pupilMethod) //change text on button
        button->setText("using Ellipse, click to switch to hough circle");
    else
        button->setText("using Hough cricle, click to switch to Ellipse");
}

