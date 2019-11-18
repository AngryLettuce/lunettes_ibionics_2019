#ifndef EYEWORLDTAB_H
#define EYEWORLDTAB_H

#include <iostream>
#include <Qt>
#include <QTabWidget>
#include <QLayout>
#include <QTimer>
#include <QLabel>
 #include <QPushButton>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

//#include "mainwindow.h"
#include "crop.h"
#include "edgeDetection.h"
#include "ellipsefit.h"
#include "houghCircle.h"
#include "modes.h"

//class MainWindow; //foward declaration to avoid circular dependencies with mainwindow.h

class EyeWorldTab : public QWidget
{
    Q_OBJECT
public:
    explicit EyeWorldTab(QWidget *parent = nullptr);

    cv::VideoCapture camEye;
    cv::VideoCapture camWorld;

    cv::Mat imgEye;
    cv::Mat imgWorld;

    QImage qimgEye;
    QImage qimgWorld;

    QTimer *tmrTimerEye;
    QTimer *tmrTimerWorld;

    QLabel *imgLblEye;
    QLabel *imgLblWorld;
    
    QPushButton *button;

    int posX;
    int posY;
    bool pupilMethod = true;

signals:

public slots:
    void processFrameEye();
    void processFrameWorld();
    void handleButton();

};

#endif // EYEWORLDTAB_H
