#include "mainwindow.h"
#include "memstab.h"
#include "lasertab.h"
#include "eyetab.h"
#include "worldtab.h"
#include "gpiotab.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    std::cout<<"debut const mainWindow"<<std::endl;

    QWidget *centralWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(centralWidget);
    QTabWidget *tabs = new QTabWidget(centralWidget);

    std::cout<<"before tabs"<<std::endl;

    memsTab = new MemsTab(tabs);
    laserTab = new LaserTab(tabs);
    std::cout<<"after laser tab"<<std::endl;
    eyeCamTab = new EyeTab(tabs);
    worldCamTab = new WorldTab(tabs);
    gpioTab = new GPIOTab();

    std::cout<<"after tabs"<<std::endl;

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
        gpioTab->spi2->inUse(true);
        gpioTab->memsEnable->inUse(true);
        /*
        worldCamTab->viewfinder->close();
        eyeCamTab->viewfinder->close();
        eyeCamTab->camera->stop();
        worldCamTab->camera->stop();
        */
    }

    else if (currentIndex == laserIndex)
    {
        gpioTab->laserEnable->inUse(true);
        gpioTab->laserEnCal->inUse(true);
        gpioTab->laserEnChannelLow->inUse(true);
        gpioTab->laserEnChannelMid->inUse(true);
        gpioTab->laserEnChannelHigh->inUse(true);
        gpioTab->laserRegen->inUse(true);
        gpioTab->i2c0->inUse(true);
        /*
        worldCamTab->viewfinder->close();
        eyeCamTab->viewfinder->close();
        eyeCamTab->camera->stop();
        worldCamTab->camera->stop();
        */
    }

    else if(currentIndex == eyeCamIndex)
    {

        gpioTab->i2c1->inUse(true);
        /*
        worldCamTab->camera->stop();
        worldCamTab->viewfinder->close();
        eyeCamTab->viewfinder->show();
        eyeCamTab->camera->start();
        */
    }

    else if(currentIndex == worldCamIndex)
    {
        gpioTab->i2c1->inUse(true);
        /*
        eyeCamTab->camera->stop();
        worldCamTab->viewfinder->show();
        eyeCamTab->viewfinder->close();
        worldCamTab->camera->start();
        */
    }

    else //If somehow no tab is selected
    {
        /*
        worldCamTab->viewfinder->close();
        eyeCamTab->viewfinder->close();
        eyeCamTab->camera->stop();
        worldCamTab->camera->stop();
        */
    }
}

