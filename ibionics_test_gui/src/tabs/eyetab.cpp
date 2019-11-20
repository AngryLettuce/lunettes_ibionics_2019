#include "eyetab.h"
#include "ellipsefit.h"
#include "houghCircle.h"


EyeTab::EyeTab(QWidget *parent, QWidget *mW) : QWidget(parent)
{
    mainPtr = reinterpret_cast<MainWindow*>(mW);
    //Layout
    QGridLayout *layout = new QGridLayout(this);
    imgLbl = new QLabel("test",this);
    //imgLbl->setGeometry(0,0,640,480);

    //Placement in Layout
    layout->addWidget(imgLbl,0,0,1,1);

    cam.open(1);//2

    if(!cam.isOpened())
    {
        std::cout<<"Error EyeCam not accessible"<<std::endl;
        return;
    }

    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrame()));
    tmrTimer->start(20);
}

void EyeTab::processFrame()
{
    cam.read(img);
    if(img.empty()) return;
    int posX = 0;
    int posY = 0;
    cv::Mat img2 = img;
    cv::cvtColor(img,img2,cv::COLOR_RGB2GRAY);
    applyEllipseMethod(img2, posX, posY);
    mainPtr->posX = posX;
    mainPtr->posY = posY;

    //Place found center on image
    cv::Point centre;
    centre.x = posX;
    centre.y = posY;
    cv::circle(img, centre,7, (255, 255, 255), -1);

    cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
    QImage qimg(reinterpret_cast<uchar*>(img.data), img.cols, img.rows, img.step, QImage::Format_RGB888);


    imgLbl->setPixmap(QPixmap::fromImage(qimg));
}
