#include "calibrationtab.h"

#define VCOS_ALIGN_DOWN(p,n) (((ptrdiff_t)(p)) & ~((n)-1))
#define VCOS_ALIGN_UP(p,n) VCOS_ALIGN_DOWN((ptrdiff_t)(p)+(n)-1,(n))

CalibrationTab::CalibrationTab(QWidget *parent, MainWindow* mW) : QWidget(parent)
{
    //TODO Init size of config in init file
    rows = 4;
    columns = 5;
    
    layout = new QGridLayout(this);
    imgLblEye = new MediaLabel(this, "");
    button = new QPushButton("Start Calibration", this);

    layout->addWidget(imgLblEye,1,0);
    layout->addWidget(button,0,0);

    connect(button, SIGNAL (clicked()), this, SLOT (startCalibration()));
    mainWindowPtr = mW;
}

void CalibrationTab::processCalibrationFrame()
{
#ifdef __arm__
    imgEye = *getImage(0, 640, 480);
#endif
#ifdef WIN32
    (mainWindowPtr->camEye).read(imgEye);
#endif
    if(imgEye.empty()) return;

    //Get corners of roi
    cv::Point upLeft = cv::Point(imgLblEye->posX - 200,imgLblEye->posY - 200);
    cv::Point downRight = cv::Point(imgLblEye->posX + 200,imgLblEye->posY + 200);
    
    //Add grid to roi
    /*
    int height = imgEye.size().height;
    int width = imgEye.size().width;
    int heightSpace = height/(rows - 1);
    int widthSpace = width/(columns - 1);
    */
    
    int leftSide = upLeft.x;
    int upSide = upLeft.y;
    
    int rightSide = downRight.x;
    int downSide = downRight.y;
    
    int heightSpace = (rightSide-leftSide)/(rows - 1);
    int widthSpace = (downSide-upSide)/(columns - 1);
    

    for (int i = upSide; i<downSide; i += heightSpace)
        cv::line(imgEye, cv::Point(leftSide, i), cv::Point(rightSide, i), cv::Scalar(0, 255, 255));

    for (int i = leftSide; i<rightSide; i += widthSpace)
        cv::line(imgEye, cv::Point(i, upSide), cv::Point(i, downSide), cv::Scalar(255, 0, 255));
    
    /*
    for(int i = 0; i<height; i = i + heightSpace) //Each row

    for(int i = 0; i<width; i = i + widthSpace) //Each column
        for(int j = 0; j<=height; j++) //Each pixel in column
            cv::line(imgEye,Point());
    */
    
    cv::cvtColor(imgEye,imgEye,cv::COLOR_BGR2RGB);
    cv::rectangle(imgEye,cv::Rect(upLeft,downRight) , cv::Scalar(0,255,0), 1, 8,0 );
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));   
}

void CalibrationTab::processPressedKey()
{
    
}

void CalibrationTab::startCalibration()
{
    
}

cv::Mat* CalibrationTab::getImage(int camNumber, int width, int height)
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

