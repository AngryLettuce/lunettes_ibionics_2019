#include "eyeworldtab.h"
 #include <QCoreApplication>

EyeWorldTab::EyeWorldTab(QWidget *parent) : QWidget(parent)
{
    posX = 0;
    posY = 0;
    //Layout
    QGridLayout *layout = new QGridLayout(this);
    imgLblEye = new QLabel("EYE",this);
    imgLblWorld = new QLabel("WORLD",this);
    
    button = new QPushButton(this);
    button->setText("using Ellipse, click to switch to hough circle");

    //Placement in Layout
    layout->addWidget(imgLblEye,1,0,1,1);
    layout->addWidget(imgLblWorld,1,1,1,1);
    
    layout->addWidget(button,0,0,1,2);
    connect(button, SIGNAL (released()), this, SLOT (handleButton()));


    camEye.open(0);//2 for webcam
    camWorld.open(2);

    if(!camEye.isOpened())
    {
        std::cout<<"Error EyeCam not accessible"<<std::endl;
        return;
    }
    if(!camWorld.isOpened())
    {
        std::cout<<"Error WorldCam not accessible"<<std::endl;
        return;
    }

    tmrTimerEye = new QTimer(this);
    connect(tmrTimerEye, SIGNAL(timeout()), this, SLOT(processFrameEye()));
    tmrTimerEye->start(33);
    
    tmrTimerWorld = new QTimer(this);
    connect(tmrTimerWorld, SIGNAL(timeout()), this, SLOT(processFrameWorld()));
    tmrTimerWorld->start(33);//33 ms default
}

void EyeWorldTab::processFrameEye()
{
    camEye.read(imgEye);
    if(imgEye.empty()) return;

    cv::Mat img2Eye = imgEye;


    cv::cvtColor(imgEye,img2Eye,cv::COLOR_RGB2GRAY);


    if(pupilMethod)
    {
        applyEllipseMethod(img2Eye, posX, posY);
    }
    else
    {
        applyHoughMethod(img2Eye,posX,posY);
    }


    //Add point to EyeCam
    cv::Point centre;
    centre.x = posX;
    centre.y = posY;
    cv::circle(imgEye, centre,7, cv::Scalar(255, 0, 0), -1);

    cv::cvtColor(imgEye,imgEye,cv::COLOR_BGR2RGB);
    //cv::cvtColor(img2World,img2World,cv::COLOR_BGR2RGB);
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    
    //QImage qimg(reinterpret_cast<uchar*>(img.data), img.cols, img.rows, img.step, QImage::Format_RGB888);

    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));
    
}

void EyeWorldTab::processFrameWorld()
{

    camWorld.read(imgWorld);
    if(imgWorld.empty()) return;


    cv::Mat img2World = imgWorld;

    cv::cvtColor(imgWorld,img2World,cv::COLOR_RGB2GRAY);


    if(RECTSHOW)
    {
        //show only crop
        cropRegion(imgWorld, &img2World, posX, posY, 160, 180);

    }
    else
    {
        //show all with rectancle
        cropRegionShow(imgWorld, &img2World, posX, posY, 160, 180);

        //cv::cvtColor(imgWorld,imgWorld,cv::COLOR_BGR2RGB);
        //QImage qimg(reinterpret_cast<uchar*>(img.data), img.cols, img.rows, img.step, QImage::Format_RGB888);
        //img2World = imgWorld.clone();

    }

    //cv::cvtColor(img2World,img2World,cv::COLOR_BGR2RGB);
    cv::cvtColor(imgWorld,imgWorld,cv::COLOR_BGR2RGB);
    //QImage qimgWorld(reinterpret_cast<uchar*>(img2World.data), img2World.cols, img2World.rows, img2World.step, QImage::Format_RGB888);
    QImage qimgWorld(reinterpret_cast<uchar*>(imgWorld.data), imgWorld.cols, imgWorld.rows, imgWorld.step, QImage::Format_RGB888);
    //QImage qimg(reinterpret_cast<uchar*>(img.data), img.cols, img.rows, img.step, QImage::Format_RGB888);

    imgLblWorld->setPixmap(QPixmap::fromImage(qimgWorld));
}


void EyeWorldTab::handleButton()
{   
        //change stage of pupil detection
        pupilMethod = !pupilMethod;
        
        if(VERBOSE)
        {
            if(pupilMethod)
                std::cout << "Change pupil detection method to Ellipse" << std::endl;
            else
                std::cout << "Change pupil detection method to hough circle" << std::endl;
        }
        
        //change text on button
        if(pupilMethod)    
            button->setText("using Ellipse, click to switch to hough circle");
        else
            button->setText("using Hough cricle, click to switch to Ellipse");
}

