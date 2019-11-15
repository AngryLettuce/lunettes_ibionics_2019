#ifndef EYETAB_H
#define EYETAB_H

#include <iostream> //
#include <Qt>
#include <QTabWidget>
#include <QLayout>
#include <QTimer> //
#include <QLabel> //
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>


#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

class EyeTab : public QWidget
{
    Q_OBJECT
public:
    explicit EyeTab(QWidget *parent = nullptr);
    QCamera *camera;
    QCameraViewfinder *viewfinder;

    //added for openc cv testing
    cv::VideoCapture cam;
    cv::Mat img;

    QImage qimg;

    QTimer *tmrTimer;

    QLabel *imgLbl;
    int posX = 0;
    int posY = 0;

signals:

public slots:
    void processFrame();

};

#endif // EYETAB_H
