#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    laser_pos_control()
{
	systemConfigs = new config("C:/views/s8ibionics/ibionics_test_gui/gui_main/config.txt");

    centralWidget = new QWidget(this);
    layout = new QGridLayout(centralWidget);
    tabs = new QTabWidget(centralWidget);

    memsTab = new MemsTab(tabs, this);
    calibrationTab = new CalibrationTab(tabs, this);
    eyeWorldTab = new EyeWorldTab(tabs, this);
    gpioTab = new GPIOTab();

    //Tabs added to layout with GPIOs
    layout->addWidget(tabs,0,0);
    //layout->addWidget(gpioTab,0,1);

    //Add Qwidgets as tabs
    memsIndex = tabs->addTab(memsTab,"MEMS");
    calibrationIndex = tabs->addTab(calibrationTab,"Laser Calibration");
    eyeWorldIndex = tabs->addTab(eyeWorldTab,"Eye & World Cam");
    setCentralWidget(centralWidget);
	
    initHw();

    //Link signals to slots
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
    
    laser_pos_control.draw_rectangle(10);
    laser_pos_control.draw_spiral(10);
    laser_pos_control.draw_infinity(10);
    laser_pos_control.draw_circluarLoop(10);
}

MainWindow::~MainWindow()
{
    //TODO exit clean (close laser, events, etc...)
}

void MainWindow::tabChange(int currentIndex)
{
    disconnect(tmrTimerEye, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameEye()));
    disconnect(tmrTimerWorld, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameWorld()));
    disconnect(tmrTimerEye, SIGNAL(timeout()), calibrationTab, SLOT(processCalibrationFrame()));
        
    if(currentIndex == eyeWorldIndex){
        if(cameras->verifyCameraPresent(0))
            connect(tmrTimerEye, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameEye()));
        else
            std::cout<<"Error EyeCam not accessible"<<std::endl;

        if(cameras->verifyCameraPresent(1))
            connect(tmrTimerWorld, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameWorld()));
        else
            std::cout<<"Error WorldCam not accessible"<<std::endl;
    }
    else if(currentIndex == calibrationIndex)
    {
        if(cameras->verifyCameraPresent(0))
            connect(tmrTimerEye, SIGNAL(timeout()), calibrationTab, SLOT(processCalibrationFrame()));
        else
            std::cout<<"Error EyeCam not accessible"<<std::endl;
    }
}

void MainWindow::initHw()
{
    tmrTimerEye = new QTimer(this);
    tmrTimerWorld = new QTimer(this);
    tmrTimerEye->start(33);
    tmrTimerWorld->start(33);//33 ms default

    cameras = new systemCameras();

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

