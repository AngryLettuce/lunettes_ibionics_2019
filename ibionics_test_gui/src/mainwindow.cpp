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

    posX = 0;
    posY = 0;

    QWidget *centralWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(centralWidget);
    QTabWidget *tabs = new QTabWidget(centralWidget);

    QWidget *mW = this;
    memsTab = new MemsTab(tabs);
    laserTab = new LaserTab(tabs);
    //eyeCamTab = new EyeTab(tabs, mW);
    //worldCamTab = new WorldTab(tabs, mW);
    eyeWorldTab = new EyeWorldTab(tabs);
    //gpioTab = new GPIOTab();

    //Tabs added to layout with GPIOs
    layout->addWidget(tabs,0,0);
    //layout->addWidget(gpioTab,0,1);

    //Add Qwidgets as tabs
    memsIndex = tabs->addTab(memsTab,"MEMS");
    laserIndex = tabs->addTab(laserTab,"Laser");
    //eyeCamIndex = tabs->addTab(eyeCamTab,"Eye Cam");
    //worldCamIndex = tabs->addTab(worldCamTab,"World Cam");
    eyeWorldIndex = tabs->addTab(eyeWorldTab,"Eye & World Cam");
    setCentralWidget(centralWidget);

    //Link signals to slots
    //connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
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

