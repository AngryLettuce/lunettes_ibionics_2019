#ifndef EYEWORLDTAB_H
#define EYEWORLDTAB_H

#include <iostream>
#include <Qt>
#include <QTabWidget>
#include <QLayout>
#include <QTimer>
#include <QLabel>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "mainwindow.h"
#include "crop.h"
#include "edgeDetection.h"
#include "ellipsefit.h"
#include "houghCircle.h"

class MainWindow; //foward declaration to avoid circular dependencies with mainwindow.h

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

    QTimer *tmrTimer;

    QLabel *imgLblEye;
    QLabel *imgLblWorld;

    int posX;
    int posY;

signals:

public slots:
    void processFrame();

};

#endif // EYEWORLDTAB_H
