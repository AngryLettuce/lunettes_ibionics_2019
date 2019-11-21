#include "mainwindow.h"
#include "memstab.h"
#include "lasertab.h"
#include "eyetab.h"
#include "worldtab.h"
#include "gpiotab.h"
#include "eyeworldtab.h"
#include "config.h"

//#ifdef __arm__
#include "arducam_mipicamera.h"
//#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    systemConfigs = new config("C:/views/s8ibionics/ibionics_test_gui/gui_main/config.txt");

    centralWidget = new QWidget(this);
    layout = new QGridLayout(centralWidget);
    tabs = new QTabWidget(centralWidget);

    memsTab = new MemsTab(tabs);
    laserTab = new LaserTab(tabs);
    eyeWorldTab = new EyeWorldTab(tabs, this);
    gpioTab = new GPIOTab();

    //Tabs added to layout with GPIOs
    layout->addWidget(tabs,0,0);
    //layout->addWidget(gpioTab,0,1);

    //Add Qwidgets as tabs
    memsIndex = tabs->addTab(memsTab,"MEMS");
    laserIndex = tabs->addTab(laserTab,"Laser");
    eyeWorldIndex = tabs->addTab(eyeWorldTab,"Eye & World Cam");
    setCentralWidget(centralWidget);

    initHw();

    //Link signals to slots
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::tabChange(int currentIndex)
{
    if(currentIndex == 2){
        if(camEye.isOpened())
            connect(tmrTimerEye, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameEye()));
        else
            std::cout<<"Error EyeCam not accessible"<<std::endl;

        if(camWorld.isOpened())
            connect(tmrTimerWorld, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameWorld()));
        else
            std::cout<<"Error WorldCam not accessible"<<std::endl;
    }
    else{
        disconnect(tmrTimerEye, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameEye()));
        disconnect(tmrTimerWorld, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameWorld()));
    }
}

void MainWindow::initHw()
{
    tmrTimerEye = new QTimer(this);
    tmrTimerWorld = new QTimer(this);
    tmrTimerEye->start(33);
    tmrTimerWorld->start(33);//33 ms default

#ifdef __arm__
    camInterface.i2c_bus = 0;
    camInterface.camera_num = 0;
    camInterface.sda_pins[0] = 28;
    camInterface.sda_pins[1] = 0;
    camInterface.scl_pins[0] = 29;
    camInterface.scl_pins[1] = 1;
    camInterface.led_pins[0] = 30;
    camInterface.led_pins[1] = 2;
    camInterface.shutdown_pins[0] = 31;
    camInterface.shutdown_pins[1] = 3;

    if(arducam_init_camera2(arducamInstance0, camInterface)){
        std::cout << "Cam1 Initialized (EyeCam)";
        arducam_set_mode(arducamInstance0, 0);
    }

    camInterface.camera_num = 1;
    if(arducam_init_camera2(arducamInstance1, camInterface)){
        std::cout << "Cam2 Initialized (WorldCam)";
        arducam_set_mode(arducamInstance1, 1);
    }
#endif
//#ifdef WIN32
    camEye.open(0);//2 for webcam
    camWorld.open(1);
//#endif
}

int MainWindow::getPosX()
{
    return posX;
}

int MainWindow::getPosY()
{
    return posY;
}
void MainWindow::setPosX(int x)
{
    posX = x;
}
void MainWindow::setPosY(int y)
{
    posY = y;
}

