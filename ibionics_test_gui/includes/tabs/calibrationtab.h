#ifndef CALIBRATIONTAB_H
#define CALIBRATIONTAB_H

#include <iostream>
#include <QLayout>
#include <QTimer>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <opencv2/core.hpp>

#include "mainwindow.h"
#include "modes.h"
#include "medialabel.h"

class MainWindow; //foward declaration to avoid circular dependencies with mainwindow.h
class MediaLabel; //foward declaration to avoid circular dependencies with medialabel.h

class CalibrationTab : public QWidget
{
    Q_OBJECT
public:
    explicit CalibrationTab(QWidget *parent = nullptr, MainWindow* mW = nullptr);
    cv::Mat* getImage(int camNumber, int width, int height);

    cv::Mat imgEye;

    MediaLabel *imgLblEye;

    QPushButton *button;
    QSlider *slider;
    QGridLayout *layout;

    int rows;
    int columns;
    int currentRow;
    int currentColumn;
    int calibrationRoiSize = 400;

    cv::Mat processedImg;
    
    MainWindow* mainWindowPtr;

signals:
    void valueChanged(int value);

public slots:
    void processCalibrationFrame();
    void processPressedKey();
    void startCalibration();
    void changeRoiSize(int size);

};

#endif // CALIBRATIONTAB_H
