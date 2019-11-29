#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	systemConfigs = new config("C:/views/s8ibionics/ibionics_test_gui/gui_main/config.txt");
    laser_pos_control = new Laser_pos_control();

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
    
    laser_pos_control->draw_rectangle(10);
    laser_pos_control->draw_spiral(10);
    laser_pos_control->draw_infinity(10);
    laser_pos_control->draw_circluarLoop(10);
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
        if((camEye.isOpened()) || (!camState0))
            connect(tmrTimerEye, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameEye()));
        else
            std::cout<<"Error EyeCam not accessible"<<std::endl;

        if((camWorld.isOpened()) || (!camState1))
            connect(tmrTimerWorld, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameWorld()));
        else
            std::cout<<"Error WorldCam not accessible"<<std::endl;
    }
    else if(currentIndex == calibrationIndex)
    {
        if(camEye.isOpened() || (!camState0))
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

#ifdef __arm__
    int cameraWidth0 = 640;
    int cameraHeight0 = 480;
    int cameraWidth1 = 640;
    int cameraHeight1 = 480;

    camInterface0.i2c_bus = 0;
    camInterface0.camera_num = 0;
    camInterface0.sda_pins[0] = 0;
    camInterface0.sda_pins[1] = 28;
    camInterface0.scl_pins[0] = 1;
    camInterface0.scl_pins[1] = 29;
    camInterface0.led_pins[0] = 2;
    camInterface0.led_pins[1] = 10;
    camInterface0.shutdown_pins[0] = 14;
    camInterface0.shutdown_pins[1] = 15;

    camState0 = arducam_init_camera2(&arducamInstance0, camInterface0);
    if(!camState0){
        std::cout << "Cam1 Initialized (EyeCam)" << std::endl;
        arducam_set_resolution(arducamInstance0, &cameraWidth0, &cameraHeight0);
    }

    camInterface1.i2c_bus = 0;
    camInterface1.camera_num = 1;
    camInterface1.sda_pins[0] = 0;
    camInterface1.sda_pins[1] = 28;
    camInterface1.scl_pins[0] = 1;
    camInterface1.scl_pins[1] = 29;
    camInterface1.led_pins[0] = 3;
    camInterface1.led_pins[1] = 10;
    camInterface1.shutdown_pins[0] = 14;
    camInterface1.shutdown_pins[1] = 15;

    camState1 = arducam_init_camera2(&arducamInstance1, camInterface1);
    if(!camState1){
        std::cout << "Cam2 Initialized (WorldCam)" << std::endl;
        arducam_set_resolution(arducamInstance1, &cameraWidth1, &cameraHeight1);

    }
#endif
#ifdef WIN32
    camEye.open(0);//2 for webcam
    camWorld.open(2);
#endif
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

