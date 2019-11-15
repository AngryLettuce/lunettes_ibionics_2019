#include "eyetab.h"
#include "ellipsefit.h"

EyeTab::EyeTab(QWidget *parent) : QWidget(parent)
{
    //Layout
    QGridLayout *layout = new QGridLayout(this);
    imgLbl = new QLabel("test",this);
    imgLbl->setGeometry(0,0,640,480);
    std::cout<<"after lbl creation"<<std::endl;
    /*
    //Checking available cameras
    camera = new QCamera();

    if (QCameraInfo::availableCameras().count() > 0)
    {
        qDebug("At least on camera was found");
        QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
        foreach (const QCameraInfo &cameraInfo, cameras)
        {

            qDebug() << cameraInfo.deviceName();
            qDebug() << cameraInfo.defaultCamera();
            if (cameraInfo.deviceName() == "EasyCamera")

            camera = new QCamera(cameraInfo);
        }
    }

    //Set up a found camera
    viewfinder = new QCameraViewfinder;
    if (camera != nullptr)
    {
        //qDebug("Setting up first camera in list");
        camera->setViewfinder(viewfinder);
        viewfinder->setParent(this);
        viewfinder->setFixedSize(400,400);
        //viewfinder->show();
        if(camera->isCaptureModeSupported(QCamera::CaptureVideo))
            camera->setCaptureMode(QCamera::CaptureVideo);
    }

    //Placement in Layout
    layout->addWidget(viewfinder,0,0);
    */

    layout->addWidget(imgLbl,0,0,1,1);

    cam.open(0);

    if(!cam.isOpened())
        std::cout<<"Error camera not accessible"<<std::endl;

    tmrTimer = new QTimer(this);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrame()));
    tmrTimer->start(20);
}

void EyeTab::processFrame()
{
    cam.read(img);
    //cv::imshow("test", img);
    if(img.empty()) return;

    cv::cvtColor(img,img,cv::COLOR_RGB2GRAY);
    applyEllipseMethod(img, posX, posY);

    //std::cout<<"col: "<<img.cols<<" rows: "<< img.rows<<std::endl;
    //std::cout<<imgLbl->width()<<std::endl;
    //QImage::Format_RGB888
    QImage qimg(reinterpret_cast<uchar*>(img.data), img.cols, img.rows, img.step, QImage::Format_Grayscale8);
    //QImage qimg((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);

    imgLbl->setPixmap(QPixmap::fromImage(qimg));
}
