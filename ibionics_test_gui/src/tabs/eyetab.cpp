#include "eyetab.h"

EyeTab::EyeTab(QWidget *parent) : QWidget(parent)
{
    //Layout
    QGridLayout *layout = new QGridLayout(this);

    //Checking available cameras
    camera = new QCamera();

    if (QCameraInfo::availableCameras().count() > 0)
    {
        qDebug("At least on camera was found");
        QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
        foreach (const QCameraInfo &cameraInfo, cameras)
        {
            /*
            qDebug() << cameraInfo.deviceName();
            qDebug() << cameraInfo.defaultCamera();
            if (cameraInfo.deviceName() == "EasyCamera")
            */
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
}
