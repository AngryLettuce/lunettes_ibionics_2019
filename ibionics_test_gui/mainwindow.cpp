#include "mainwindow.h"
#include "memstab.h"
#include "lasertab.h"
#include "eyetab.h"
#include "worldtab.h"
#include "gpiotab.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QWidget *centralWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(centralWidget);
    QTabWidget *tabs = new QTabWidget(centralWidget);

    memsTab = new MemsTab(tabs);
    laserTab = new LaserTab(tabs);
    eyeCamTab = new EyeTab(tabs);
    worldCamTab = new WorldTab(tabs);
    gpioTab = new GPIOTab();


    //Tabs added to layout with GPIOs
    layout->addWidget(tabs,0,0);
    layout->addWidget(gpioTab,0,1);

    //Add Qwidgets as tabs
    memsIndex = tabs->addTab(memsTab,"MEMS");
    laserIndex = tabs->addTab(laserTab,"Laser");
    eyeCamIndex = tabs->addTab(eyeCamTab,"Eye Cam");
    worldCamIndex = tabs->addTab(worldCamTab,"World Cam");
    setCentralWidget(centralWidget);

    //Link signals to slots
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::tabChange(int currentIndex)
{
    gpioTab->changeAllLabelsToRed();

    if (currentIndex == memsIndex)
    {
        gpioTab->spi2->setStyleSheet("QLabel{background-color: springgreen;}");
        gpioTab->memsEnable->setStyleSheet("QLabel{background-color: springgreen;}");

        worldCamTab->viewfinder->close();
        eyeCamTab->viewfinder->close();
        eyeCamTab->camera->stop();
        worldCamTab->camera->stop();
    }

    else if (currentIndex == laserIndex)
    {
        gpioTab->laserEnable->setStyleSheet("QLabel{background-color: springgreen;}");
        gpioTab->laserEnCal->setStyleSheet("QLabel{background-color: springgreen;}");
        gpioTab->laserEnChannelLow->setStyleSheet("QLabel{background-color: springgreen;}");
        gpioTab->laserEnChannelMid->setStyleSheet("QLabel{background-color: springgreen;}");
        gpioTab->laserEnChannelHigh->setStyleSheet("QLabel{background-color: springgreen;}");
        gpioTab->laserRegen->setStyleSheet("QLabel{background-color: springgreen;}");
        gpioTab->i2c0->setStyleSheet("QLabel{background-color: springgreen;}");

        worldCamTab->viewfinder->close();
        eyeCamTab->viewfinder->close();
        eyeCamTab->camera->stop();
        worldCamTab->camera->stop();
    }

    else if(currentIndex == eyeCamIndex)
    {

        gpioTab->i2c1->setStyleSheet("QLabel{background-color: springgreen;}");
        worldCamTab->camera->stop();
        worldCamTab->viewfinder->close();
        eyeCamTab->viewfinder->show();
        eyeCamTab->camera->start();

    }

    else if(currentIndex == worldCamIndex)
    {
        gpioTab->i2c1->setStyleSheet("QLabel{background-color: springgreen;}");
        eyeCamTab->camera->stop();
        worldCamTab->viewfinder->show();
        eyeCamTab->viewfinder->close();
        worldCamTab->camera->start();
    }

    else //If somehow no tab is selected
    {
        worldCamTab->viewfinder->close();
        eyeCamTab->viewfinder->close();
        eyeCamTab->camera->stop();
        worldCamTab->camera->stop();
    }
}

