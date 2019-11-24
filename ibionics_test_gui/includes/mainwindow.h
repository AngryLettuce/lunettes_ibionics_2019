#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

#include "memstab.h"
#include "lasertab.h"
#include "eyetab.h"
#include "worldtab.h"
#include "gpiotab.h"
#include "eyeworldtab.h"
#include "config.h"
#include "arducam_mipicamera.h"

#include "laser_pos_control.h"


class MemsTab; //foward declaration to avoid circular dependencies with memstab.h
class WorldTab; //foward declaration to avoid circular dependencies with worldtab.h
class EyeTab; //foward declaration to avoid circular dependencies with eyetab.h
class EyeWorldTab; //foward declaration to avoid circular dependencies with eyeWorldtab.h
class Laser_pos_control; //foward declaration to avoid circular dependencies with eyeWorldtab.h

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initHw();

    MemsTab *memsTab;
    LaserTab *laserTab;
    EyeTab *eyeCamTab;
    WorldTab *worldCamTab;
    GPIOTab *gpioTab;
    EyeWorldTab *eyeWorldTab;

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

private:
    int memsIndex;
    int laserIndex;
    int eyeCamIndex;
    int worldCamIndex;
    int eyeWorldIndex;

    QPixmap *qPixMapImage;

public slots:
    void tabChange(int currentIndex);
};
#endif // MAINWINDOW_H
