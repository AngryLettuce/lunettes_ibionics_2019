#include "worldtab.h"
#include "crop.h"
#include "edgeDetection.h"


WorldTab::WorldTab(QWidget *parent, QWidget *mW) : QWidget(parent)
{
    mainPtr = reinterpret_cast<MainWindow*>(mW);

    //Layout
    QGridLayout *layout = new QGridLayout(this);
    imgLbl = new QLabel("test",this);
    //imgLbl->setGeometry(0,0,640,480);

    //Placement in Layout
    layout->addWidget(imgLbl,0,0,1,1);

    cam.open(0);

    if(!cam.isOpened())
    {
        std::cout<<"Error WorldCam not accessible"<<std::endl;
        return;
    }

    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrame()));
    tmrTimer->start(20);
}

void WorldTab::processFrame()
{
    cam.read(img);
    if(img.empty()) return;

    cv::Mat img2 = img;
    //cv::cvtColor(img,img2,cv::COLOR_RGB2GRAY);

    if(RECTSHOW)
    {
        //show only crop
        cropRegion(img, &img2, mainPtr->posX, mainPtr->posX, 160, 180);

    }
    else
    {
        //std::cout << "still on showing" << std::endl;
        //show all with rectancle
        cropRegionShow(img, &img2, mainPtr->posX, mainPtr->posX, 160, 180);

        cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
        //QImage qimg(reinterpret_cast<uchar*>(img.data), img.cols, img.rows, img.step, QImage::Format_RGB888);
        img2 = img.clone();

    }

    QImage qimg(reinterpret_cast<uchar*>(img2.data), img2.cols, img2.rows, img2.step, QImage::Format_RGB888);

    //QImage qimg(reinterpret_cast<uchar*>(img.data), img.cols, img.rows, img.step, QImage::Format_RGB888);

    imgLbl->setPixmap(QPixmap::fromImage(qimg));
}

