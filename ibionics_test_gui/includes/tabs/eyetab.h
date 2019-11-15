#ifndef EYETAB_H
#define EYETAB_H

#include <iostream>
#include <Qt>
#include <QTabWidget>
#include <QLayout>
#include <QTimer>
#include <QLabel>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "mainwindow.h"

class MainWindow; //foward declaration to avoid circular dependencies with mainwindow.h

class EyeTab : public QWidget
{
    Q_OBJECT
public:
    explicit EyeTab(QWidget *parent = nullptr, QWidget *mW = nullptr);
    QCamera *camera;
    QCameraViewfinder *viewfinder;

    //added for openc cv testing
    cv::VideoCapture cam;
    cv::Mat img;

    QImage qimg;

    QTimer *tmrTimer;

    QLabel *imgLbl;

    MainWindow *mainPtr;

signals:

public slots:
    void processFrame();

};

#endif // EYETAB_H
