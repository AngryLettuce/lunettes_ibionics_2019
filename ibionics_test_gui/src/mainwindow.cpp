#include <fstream>

#include "mainwindow.h"

int stopSig = 0;
int frameBuffer = 50;
std::vector<cv::Mat> frameBufferCam0 = *new std::vector<cv::Mat>[frameBuffer*640*480];
std::vector<cv::Mat> frameBufferCam1 = *new std::vector<cv::Mat>[frameBuffer*640*480];
std::chrono::duration<double> eyeCamLatency;
std::chrono::duration<double> worldCamLatency;
systemCameras cameras;

void grabCam0Frame(void){
    cv::Mat frameEye;
    frameBufferCam0.clear();
    while(!stopSig){
        if(cameras.verifyCameraPresent(0)){
            frameEye = cameras.readImgEye();
            if(frameBufferCam0.size() > 2)
                frameBufferCam0.pop_back();
            if(frameBufferCam0.size() < frameBuffer)
                frameBufferCam0.push_back(frameEye);
            else
                frameBufferCam0.clear();
        }
        eyeCamLatency = cameras.eyeSensorLatency;
    }
}

void grabCam1Frame(void){
    cv::Mat frameWorld;
    frameBufferCam1.clear();
    while(!stopSig){
        if(cameras.verifyCameraPresent(1)){
            frameWorld = cameras.readImgWorld();
            if(frameBufferCam1.size() > 2)
                frameBufferCam1.pop_back();
            if(frameBufferCam1.size() < frameBuffer)
                frameBufferCam1.push_back(frameWorld);
            else
                frameBufferCam1.clear();
        }
        worldCamLatency = cameras.worldSensorLatency;
    }
}

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
    laser_pos_control.draw_rectangle(10);
    laser_pos_control.send_pos(CAMERA_RESOLUTION/2,CAMERA_RESOLUTION/2);
    connect(tmrTimerEye, SIGNAL(timeout()), memsTab, SLOT(processMemsFrame()));
}

MainWindow::~MainWindow()
{
    stopSig = 1;
    tEye.join();
    tWorld.join();
}

void MainWindow::tabChange(int currentIndex)
{
    disconnect(tmrTimerEye,     SIGNAL(timeout()),  eyeWorldTab,    SLOT(processFrameEye()));
    disconnect(tmrTimerWorld,   SIGNAL(timeout()),  eyeWorldTab,    SLOT(processFrameWorld()));
    disconnect(tmrTimerEye,     SIGNAL(timeout()),  calibrationTab, SLOT(processCalibrationFrame()));
    disconnect(tmrTimerEye,     SIGNAL(timeout()),  memsTab,        SLOT(processMemsFrame()));
        
    if(currentIndex == eyeWorldIndex){
        connect(tmrTimerEye,    SIGNAL(timeout()),  eyeWorldTab,    SLOT(processFrameEye()));
        connect(tmrTimerWorld,  SIGNAL(timeout()),  eyeWorldTab,    SLOT(processFrameWorld()));
    }
    else if(currentIndex == calibrationIndex){
        laser_pos_control.laser.on();
        connect(tmrTimerEye,    SIGNAL(timeout()),  calibrationTab, SLOT(processCalibrationFrame()));
    }
    else if(currentIndex == memsIndex)
    {
        (memsTab->laser_on) ? laser_pos_control.laser.on() : laser_pos_control.laser.off();
        connect(tmrTimerEye,    SIGNAL(timeout()),  memsTab,        SLOT(processMemsFrame()));
    }
}

void MainWindow::initHw()
{
    tmrTimerEye = new QTimer(this);
    tmrTimerWorld = new QTimer(this);
    tmrTimerEye->start(33);
    tmrTimerWorld->start(33);//33 ms default

    laser_pos_control.draw_rectangle(10);
    laser_pos_control.send_pos(CAMERA_RESOLUTION/2,CAMERA_RESOLUTION/2);

    tEye = std::thread(grabCam0Frame);
    tWorld = std::thread(grabCam1Frame);
    //This is an arbitrary value. A smaller delay would probably do the job
    // it let the cameras the time to boot up before the app want frames
    QThread::msleep(1000);
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
