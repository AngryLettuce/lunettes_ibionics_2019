#include "eyeworldtab.h"

EyeWorldTab::EyeWorldTab(QWidget *parent) : QWidget(parent)
{
    //Layout
    QGridLayout *layout = new QGridLayout(this);
    imgLblEye = new QLabel("EYE",this);
    imgLblWorld = new QLabel("WORLD",this);

    //Placement in Layout
    layout->addWidget(imgLblEye,0,0,1,1);
    layout->addWidget(imgLblWorld,0,1,1,1);

    camEye.open(0);
    camWorld.open(1);

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

    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrame()));
    tmrTimer->start(20);
}

void EyeWorldTab::processFrame()
{
    camEye.read(imgEye);
    if(imgEye.empty()) return;

    camWorld.read(imgWorld);
    if(imgWorld.empty()) return;

    cv::Mat img2Eye = imgEye;
    cv::Mat img2World = imgWorld;

    cv::cvtColor(imgEye,img2Eye,cv::COLOR_RGB2GRAY);
    cv::cvtColor(imgWorld,img2World,cv::COLOR_RGB2GRAY);


    applyEllipseMethod(img2Eye, posX, posY);


    if(RECTSHOW)
    {
        //show only crop
        cropRegion(imgWorld, &img2World, posX, posY, 160, 180);

    }
    else
    {
        //std::cout << "still on showing" << std::endl;
        //show all with rectancle
        cropRegionShow(imgWorld, &img2World, posX, posY, 160, 180);

        cv::cvtColor(imgWorld,imgWorld,cv::COLOR_BGR2RGB);
        //QImage qimg(reinterpret_cast<uchar*>(img.data), img.cols, img.rows, img.step, QImage::Format_RGB888);
        img2World = imgWorld.clone();

    }
    //Add point to EyeCam
    cv::Point centre;
    centre.x = posX;
    centre.y = posY;
    cv::circle(imgEye, centre,7, (255, 255, 255), -1);

    cv::cvtColor(imgEye,imgEye,cv::COLOR_BGR2RGB);
    //cv::cvtColor(img2World,img2World,cv::COLOR_BGR2RGB);
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    QImage qimgWorld(reinterpret_cast<uchar*>(img2World.data), img2World.cols, img2World.rows, img2World.step, QImage::Format_RGB888);
    //QImage qimg(reinterpret_cast<uchar*>(img.data), img.cols, img.rows, img.step, QImage::Format_RGB888);

    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));
    imgLblWorld->setPixmap(QPixmap::fromImage(qimgWorld));
}

