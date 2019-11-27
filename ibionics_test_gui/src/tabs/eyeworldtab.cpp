#include "eyeworldtab.h"

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
#ifdef __arm__
    imgEye = *getImage(0, 640, 480);
#endif
#ifdef WIN32
    (mainWindowPtr->camEye).read(imgEye);
#endif
    cropRegion(&imgEye, &imgEye, mainWindowPtr->calibrationPosX, mainWindowPtr->calibrationPosY, mainWindowPtr->roiSize, mainWindowPtr->roiSize, false);
    //cv::resize(imgEye, imgEye ,cv::Size(), 400/mainWindowPtr->roiSize, 400/mainWindowPtr->roiSize, cv::INTER_LINEAR);
    cv::resize(imgEye, imgEye ,cv::Size(400,400), 0, 0, cv::INTER_LINEAR);
    std::cout<<"Size: "<<imgEye.size()<<std::endl;
    if(imgEye.channels() <= 1){
        if(imgEye.empty()) return;
        // if the signature of the functions changes and return a cv::Point, we could make it in one line
        (pupilMethod) ? applyEllipseMethod(&imgEye, posX,posY) : applyHoughMethod(&imgEye,posX,posY) ;
        cv::circle(imgEye, cv::Point(posX,posY),7, cv::Scalar(255, 0, 0), -1);
    }
    else{
        cv::Mat img2Eye;
        cv::cvtColor(imgEye,img2Eye,cv::COLOR_RGB2GRAY);
        (pupilMethod) ? applyEllipseMethod(&img2Eye, posX,posY) : applyHoughMethod(&img2Eye,posX,posY) ;
        cv::circle(imgEye, cv::Point(posX,posY),7, cv::Scalar(255, 0, 0), -1);
        cv::cvtColor(imgEye,imgEye,cv::COLOR_BGR2RGB);
    }
	
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));   
    
    //Adjust laser position 
	//TODO Hot Fix
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
    //std::cout<<"PosX: "<<posX<<"PosY: "<<posY<<std::endl;
    
    mainWindowPtr->laser_pos_control->send_pos(posY, posX);
}

void EyeWorldTab::processFrameWorld()
{
#ifdef __arm__
    imgWorld = *getImage(1, 640, 480);
#endif
#ifdef WIN32
    (mainWindowPtr->camWorld).read(imgWorld);
#endif
    cv::Mat img2World = imgWorld;
	if(imgWorld.empty()) return;
    if(imgWorld.channels() <= 1){
        if(imgWorld.empty()) return;
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

