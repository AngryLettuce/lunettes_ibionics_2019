#ifndef CALIBRATIONTAB_H
#define CALIBRATIONTAB_H

#include <iostream>
#include <QLayout>
#include <QTimer>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <opencv2/core.hpp>
#include <QKeyEvent>

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
    void keyPressEvent(QKeyEvent *event);
    cv::Mat imgEye;

    MediaLabel *imgLblEye;

    QPushButton *button;
    QSlider *slider;
    QGridLayout *layout;

    int rows;
    int columns;
    int currentRow;
    int currentColumn;

    cv::Mat processedImg;
    
    MainWindow* mainWindowPtr;

    float momentum;
    int lastKeyPressed = 0;
    int currentKey = 0;
    const float delta_angle = 0.01;
    float angle_x;
    float angle_y;
    int row_calib_counter;
    int column_calib_counter;
    
    bool inCalibration = false;

signals:
    void valueChanged(int value);

public slots:
    void processCalibrationFrame();
    void startCalibration();

};

#endif // CALIBRATIONTAB_H
