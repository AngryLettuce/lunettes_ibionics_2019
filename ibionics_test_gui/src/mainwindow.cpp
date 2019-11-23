#include "mainwindow.h"
#include "memstab.h"
#include "lasertab.h"
#include "eyetab.h"
#include "worldtab.h"
#include "gpiotab.h"
#include "eyeworldtab.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    config* asdf = new config("C:/views/s8ibionics/ibionics_test_gui/gui_main/config.txt");

    QWidget *centralWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(centralWidget);
    QTabWidget *tabs = new QTabWidget(centralWidget);

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

    tmrTimerEye = new QTimer(this);
    tmrTimerWorld = new QTimer(this);
    tmrTimerEye->start(33);
    tmrTimerWorld->start(33);//33 ms default
    camEye.open(1);//2 for webcam
    camWorld.open(0);

    //Link signals to slots
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::tabChange(int currentIndex)
{
        disconnect(tmrTimerEye, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameEye()));
        disconnect(tmrTimerWorld, SIGNAL(timeout()), eyeWorldTab, SLOT(processFrameWorld()));
    
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

