#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

#include "memstab.h"
#include "gpiotab.h"
#include "eyeworldtab.h"
#include "calibrationtab.h"

#include "config.h"
#include "laser_pos_control.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "arducam_mipicamera.h"

class MemsTab; //foward declaration to avoid circular dependencies with memstab.h
class EyeWorldTab; //foward declaration to avoid circular dependencies with eyeWorldtab.h
class CalibrationTab; //foward declaration to avoid circular dependencies with calibrationtab.h
class Laser_pos_control; //foward declaration to avoid circular dependencies with eyeWorldtab.h

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initHw();

    MemsTab *memsTab;
    GPIOTab *gpioTab;
    EyeWorldTab *eyeWorldTab;
    CalibrationTab *calibrationTab;

    QWidget *centralWidget;
    QGridLayout *layout;
    QTabWidget *tabs;

    int posX = 0;
    int posY = 0;

    int getPosX();
    int getPosY();
    void setPosX(int x);
    void setPosY(int y);

    QTimer *tmrTimerEye;
    QTimer *tmrTimerWorld;

    cv::VideoCapture camEye;
    cv::VideoCapture camWorld;
    
    Laser_pos_control* laser_pos_control;

    config* systemConfigs;
    camera_interface camInterface;
    CAMERA_INSTANCE arducamInstance0;
    CAMERA_INSTANCE arducamInstance1;
    int camState0 = 0;
    int camState1 = 0;

    cv::Point upLeft  = cv::Point(0,0);
    cv::Point downRight = cv::Point(1,1);

    int calibrationPosX = 0;
    int calibrationPosY = 0;

    int roiSize = 400;
    int leftSide;
    int upSide;
    int rightSide;
    int downSide;

private:
    int memsIndex;
    int laserIndex;
    int eyeCamIndex;
    int worldCamIndex;
    int eyeWorldIndex;
    int calibrationIndex;

    QPixmap *qPixMapImage;

public slots:
    void tabChange(int currentIndex);
};
#endif // MAINWINDOW_H
