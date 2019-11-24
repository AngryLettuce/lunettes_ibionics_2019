#include "eyeworldtab.h"
#include <QCoreApplication>
#include "mainwindow.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#define VCOS_ALIGN_DOWN(p,n) (((ptrdiff_t)(p)) & ~((n)-1))
#define VCOS_ALIGN_UP(p,n) VCOS_ALIGN_DOWN((ptrdiff_t)(p)+(n)-1,(n))

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
}

void EyeWorldTab::processFrameEye()
{
    int width = 640;
    int height = 480;
#ifdef __arm__
    imgEye = *getImage(0, width, height);
    //imgEye = *imgCamEye;
#endif
#ifdef WIN32
    (mainWindowPtr->camEye).read(imgEye);
#endif
    if(imgEye.empty()) return;

    cv::Mat img2Eye;
    if(imgEye.channels() > 1)
        cv::cvtColor(imgEye,img2Eye,cv::COLOR_RGB2GRAY);
    else
        img2Eye = imgEye;

    if(pupilMethod)
        applyEllipseMethod(img2Eye, posX, posY);
    else
        applyHoughMethod(img2Eye,posX,posY);

    //Add point to EyeCam
    cv::Point centre = cv::Point(posX,posY);
    cv::circle(imgEye, centre,7, cv::Scalar(255, 0, 0), -1);

    cv::cvtColor(imgEye,imgEye,cv::COLOR_BGR2RGB);
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));   
}

void EyeWorldTab::processFrameWorld()
{
    
    int width = 640;
    int height = 480;
#ifdef __arm__
    imgWorld = *getImage(1, width, height);
    //imgWorld = *imgCamWorld;
#endif
#ifdef WIN32
    (mainWindowPtr->camWorld).read(imgWorld);
#endif
    if(imgWorld.empty()) return;

    cv::Mat img2World = imgWorld;
    cv::cvtColor(imgWorld,img2World,cv::COLOR_RGB2GRAY);

    if(RECTSHOW)
        cropRegion(imgWorld, &img2World, posX, posY, 160, 180); //show only crop
    else
    {
        cropRegionShow(imgWorld, &img2World, posX, posY, 160, 180); //show all with rectancle
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

cv::Mat* EyeWorldTab::getImage(int camNumber, int width, int height)
{

    IMAGE_FORMAT fmt = {IMAGE_ENCODING_I420, 100};
    BUFFER *buffer = nullptr;
#ifdef __arm__
    if(camNumber == 0)
        buffer = arducam_capture(mainWindowPtr->arducamInstance0, &fmt, 3000);
    else
        buffer = arducam_capture(mainWindowPtr->arducamInstance1, &fmt, 3000);
#endif
    if (!buffer)
        return nullptr;
#ifdef __arm__
    // The actual width and height of the IMAGE_ENCODING_RAW_BAYER format and the IMAGE_ENCODING_I420 format are aligned,
    // width 32 bytes aligned, and height 16 byte aligned.
    width = VCOS_ALIGN_UP(width, 32);
    height = VCOS_ALIGN_UP(height, 16);
    processedImg = cv::Mat(cv::Size(width,(int)(height * 1.5)), CV_8UC1, buffer->data);
    cv::cvtColor(processedImg, processedImg, cv::COLOR_YUV2BGR_I420);
    arducam_release_buffer(buffer);
    return &processedImg;
#endif
    return nullptr;
}

