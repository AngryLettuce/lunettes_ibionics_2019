#include "gpiotab.h"

GPIOTab::GPIOTab(QWidget *parent) : QWidget(parent)
{
    // Main Layout
    QGridLayout *gpioLayout = new QGridLayout(this);

    // Sections Widget
    mems = new QWidget(this);
    laser = new QWidget(this);
    pot = new QWidget(this);
    camera = new QWidget(this);
    aiy = new QWidget(this);

    // Sections Layouts
    layoutMems = new QVBoxLayout(mems);
    layoutLaser = new QVBoxLayout(laser);
    layoutPot = new QVBoxLayout(pot);
    layoutCamera = new QVBoxLayout(camera);
    layoutAiy = new QVBoxLayout(aiy);

    //Sections Labels
    memsLbl = new QLabel("MEMS", this);
    laserLbl = new QLabel("Laser", this);
    potLbl = new QLabel("Digital Potentiometer", this);
    cameraLbl = new QLabel("Camera", this);
    aiyLbl = new QLabel("AIY", this);

    memsLbl->setStyleSheet("border-bottom-width: 1px; border-bottom-style: solid; border-radius: 1px;");
    laserLbl->setStyleSheet("border-bottom-width: 1px; border-bottom-style: solid; border-radius: 1px;");
    potLbl->setStyleSheet("border-bottom-width: 1px; border-bottom-style: solid; border-radius: 1px;");
    cameraLbl->setStyleSheet("border-bottom-width: 1px; border-bottom-style: solid; border-radius: 1px;");
    aiyLbl->setStyleSheet("border-bottom-width: 1px; border-bottom-style: solid; border-radius: 1px;");

    // MEMS
    spi2 = new GpioView("spi2", mems);
    memsEnable = new GpioView("GPIO 39 MEMS Enable", mems);

    // Laser
    laserEnable = new GpioView("GPIO 32 Laser Enable", laser);
    laserEnCal = new GpioView("GPIO 30 Laser Calibration Enable", laser);
    laserEnChannelLow = new GpioView("GPIO 38 Enable Channel Low", laser);
    laserEnChannelMid = new GpioView("GPIO 37 Enable Channel Mid", laser);
    laserEnChannelHigh = new GpioView("GPIO 33 Enable Channel High", laser);
    laserRegen = new GpioView("GPIO 31 Regulator Enable", laser);

    // Digital Potentiometer
    i2c0 = new GpioView("I2C 0", pot);

    // Camera
    i2c1 = new GpioView("I2C 1", camera);

    // AIY
    i2c0Aiy = new GpioView("I2C 0", aiy);
    spi2Aiy = new GpioView("spi2", aiy);

    // Placement in Layout
    layoutMems->addWidget(spi2);
    layoutMems->addWidget(memsEnable);

    layoutLaser->addWidget(laserEnable);
    layoutLaser->addWidget(laserEnCal);
    layoutLaser->addWidget(laserEnChannelLow);
    layoutLaser->addWidget(laserEnChannelMid);
    layoutLaser->addWidget(laserEnChannelHigh);
    layoutLaser->addWidget(laserRegen);

    layoutPot->addWidget(i2c0);

    layoutCamera->addWidget(i2c1);

    layoutAiy->addWidget(i2c0Aiy);
    layoutAiy->addWidget(spi2Aiy);

    gpioLayout->addWidget(memsLbl);
    gpioLayout->addWidget(mems);
    gpioLayout->addWidget(laserLbl);
    gpioLayout->addWidget(laser);
    gpioLayout->addWidget(potLbl);
    gpioLayout->addWidget(pot);
    gpioLayout->addWidget(cameraLbl);
    gpioLayout->addWidget(camera);
    gpioLayout->addWidget(aiyLbl);
    gpioLayout->addWidget(aiy);

    //Turn on only MEMS IOs for init
    changeAllLabelsToRed();
    spi2->inUse(true);
    memsEnable->inUse(true);
}

GPIOTab::~GPIOTab()
{

}

void GPIOTab::changeAllLabelsToRed()
{
    GpioView *labelToUpdate;
    QList<QWidget*> sectionList = this->findChildren<QWidget*>();
    foreach (auto section, sectionList)
    {
        QList<GpioView*> gpioLabelList = section->findChildren<GpioView*>();

        foreach (auto gpiolabel, gpioLabelList)
        {
            labelToUpdate = reinterpret_cast<GpioView*>(gpiolabel); //Change pointer type from QWidget to QLabel
            labelToUpdate->inUse(false);
        }
    }
}
