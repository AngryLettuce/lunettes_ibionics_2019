#include <fstream>

#include "mainwindow.h"

#define CALIBRATION_GRID_PARAMS_FILENAME "/home/pi/Desktop/s8ibionics/ibionics_test_gui/gui_main/calibrationGridParams.txt"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    laser_pos_control()
{
	systemConfigs = new config("C:/views/s8ibionics/ibionics_test_gui/gui_main/config.txt");
    
    loadCalibrationGridParams();
    
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
    
    //Considering it start on the mems tab
    if(cameras->verifyCameraPresent(0))
        connect(tmrTimerEye, SIGNAL(timeout()), memsTab, SLOT(processMemsFrame()));
    else
        std::cout<<"Error EyeCam not accessible"<<std::endl;
    
    laser_pos_control.draw_rectangle(10);
    laser_pos_control.send_pos(CAMERA_RESOLUTION/2,CAMERA_RESOLUTION/2);
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
    disconnect(tmrTimerEye, SIGNAL(timeout()), memsTab, SLOT(processMemsFrame()));
        
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
    else if(currentIndex == memsIndex)
    {
        if(cameras->verifyCameraPresent(0))
            connect(tmrTimerEye, SIGNAL(timeout()), memsTab, SLOT(processMemsFrame()));
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

void MainWindow::saveCalibrationGridParams() {
    std::ofstream myfile(CALIBRATION_GRID_PARAMS_FILENAME);
    if(myfile.fail()) {
    }
    else {
        myfile << calibrationPosX << std::endl;
        myfile << calibrationPosY << std::endl;
        myfile << roiSize << std::endl;
    }
}

void MainWindow::loadCalibrationGridParams() {
    std::ifstream myfile(CALIBRATION_GRID_PARAMS_FILENAME);
    if(myfile.fail()) {
        calibrationPosX = 0;
        calibrationPosY = 0;
        roiSize = 400;
    }
    else {
        
        myfile >> calibrationPosX;
        myfile >> calibrationPosY;
        myfile >> roiSize;
    }
}
