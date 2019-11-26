#ifndef EYEWORLDTAB_H
#define EYEWORLDTAB_H

#include <iostream>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <opencv2/core.hpp>

#include "mainwindow.h"
#include "crop.h"
#include "edgeDetection.h"
#include "ellipsefit.h"
#include "houghCircle.h"
#include "modes.h"

class MainWindow; //foward declaration to avoid circular dependencies with mainwindow.h
class Laser_pos_control; //foward declaration to avoid circular dependencies with mainwindow.h

class EyeWorldTab : public QWidget
{
    Q_OBJECT
public:
    explicit EyeWorldTab(QWidget *parent = nullptr, MainWindow* mW = nullptr);
    cv::Mat* getImage(int camera_instance, int width, int height);

    cv::Mat imgEye;
    cv::Mat imgWorld;

    QLabel *imgLblEye;
    QLabel *imgLblWorld;
    
    QPushButton *button;
    QGridLayout *layout;

    int posX = 0;
    int posY = 0;
    bool pupilMethod = true;
    MainWindow* mainWindowPtr;

    cv::Point pupilPos;
    cv::Mat processedImg;

signals:

public slots:
    void processFrameEye();
    void processFrameWorld();
    void switchPupilMethodButton();

};

#endif // EYEWORLDTAB_H
