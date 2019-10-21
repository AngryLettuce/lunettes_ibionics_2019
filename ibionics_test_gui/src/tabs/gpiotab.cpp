#include "includes/tabs/gpiotab.h"

GPIOTab::GPIOTab(QWidget *parent) : QWidget(parent)
{
    //Main Layout
    QGridLayout *layout = new QGridLayout(this);

    //Widgets
    QWidget *mems = new QWidget(this);
    QWidget *laser = new QWidget(this);
    QWidget *pot = new QWidget(this);
    QWidget *camera = new QWidget(this);
    QWidget *aiy = new QWidget(this);

    //Sections Layouts
    QGridLayout *layoutMems = new QGridLayout(mems);
    QGridLayout *layoutLaser = new QGridLayout(laser);
    QGridLayout *layoutPot = new QGridLayout(pot);
    QGridLayout *layoutCamera = new QGridLayout(camera);
    QGridLayout *layoutAiy = new QGridLayout(aiy);

    //Sections Labels
    /*
    QLabel *memsLbl = new QLabel("MEMS", mems);
    QLabel *laserLbl = new QLabel("Laser", laser);
    QLabel *potLbl = new QLabel("Digital Potentiometer", pot);
    QLabel *cameraLbl = new QLabel("Camera", camera);
    QLabel *aiyLbl = new QLabel("AIY", aiy);
    */
    QLabel *memsLbl = new QLabel("MEMS", this);
    QLabel *laserLbl = new QLabel("Laser", this);
    QLabel *potLbl = new QLabel("Digital Potentiometer", this);
    QLabel *cameraLbl = new QLabel("Camera", this);
    QLabel *aiyLbl = new QLabel("AIY", this);

    memsLbl->setStyleSheet("border-bottom-width: 1px; border-bottom-style: solid; border-radius: 1px;");
    laserLbl->setStyleSheet("border-bottom-width: 1px; border-bottom-style: solid; border-radius: 1px;");
    potLbl->setStyleSheet("border-bottom-width: 1px; border-bottom-style: solid; border-radius: 1px;");
    cameraLbl->setStyleSheet("border-bottom-width: 1px; border-bottom-style: solid; border-radius: 1px;");
    aiyLbl->setStyleSheet("border-bottom-width: 1px; border-bottom-style: solid; border-radius: 1px;");


    //mems
    spi2 = new QLabel("spi2", mems);
    memsEnable = new QLabel("GPIO 39 MEMS Enable", mems);

    //Laser
    laserEnable = new QLabel("GPIO 32 Laser Enable", laser);
    laserEnCal = new QLabel("GPIO 30 Laser Calibration Enable", laser);
    laserEnChannelLow = new QLabel("GPIO 38 Enable Channel Low", laser);
    laserEnChannelMid = new QLabel("GPIO 37 Enable Channel Mid", laser);
    laserEnChannelHigh = new QLabel("GPIO 33 Enable Channel High", laser);
    laserRegen = new QLabel("GPIO 31 Regulator Enable", laser);

    //digital potentiometer
    i2c0 = new QLabel("I2C 0", pot);

    //camera
    i2c1 = new QLabel("I2C 1", camera);

    //AIY
    i2c0Aiy = new QLabel("I2C 0", aiy);
    spi2Aiy = new QLabel("spi2", aiy);

    //i2c0->setStyleSheet("background-color: red;");

    //QSpacerItem *spacer = new QSpacerItem(1,100);

    //Placement in Layout
    layoutMems->addWidget(spi2,1,0);
    layoutMems->addWidget(memsEnable,2,0);

    layoutLaser->addWidget(laserEnable,1,0);
    layoutLaser->addWidget(laserEnCal,2,0);
    layoutLaser->addWidget(laserEnChannelLow,3,0);
    layoutLaser->addWidget(laserEnChannelMid,4,0);
    layoutLaser->addWidget(laserEnChannelHigh,5,0);
    layoutLaser->addWidget(laserRegen,6,0);

    layoutPot->addWidget(i2c0,1,0);

    layoutCamera->addWidget(i2c1,1,0);

    layoutAiy->addWidget(i2c0Aiy,1,0);
    layoutAiy->addWidget(spi2Aiy,2,0);

    layout->addWidget(memsLbl,0,0);
    layout->addWidget(mems,1,0);
    layout->addWidget(laserLbl,2,0);
    layout->addWidget(laser,3,0);
    layout->addWidget(potLbl,4,0);
    layout->addWidget(pot,5,0);
    layout->addWidget(cameraLbl,6,0);
    layout->addWidget(camera,7,0);
    layout->addWidget(aiyLbl,8,0);
    layout->addWidget(aiy,9,0);

    //Turn on only MEMS IOs for init
    changeAllLabelsToRed();
    spi2->setStyleSheet("QLabel{background-color: springgreen;}");
    memsEnable->setStyleSheet("QLabel{background-color: springgreen;}");
}

GPIOTab::~GPIOTab()
{

}

void GPIOTab::changeAllLabelsToRed()
{
    QLabel *labelGood;
    QList<QWidget*> sectionList = this->findChildren<QWidget*>();
        foreach (auto section, sectionList)
        {
            QList<QWidget*> labelList = section->findChildren<QWidget*>();

            foreach (auto label, labelList)
            {
                labelGood = reinterpret_cast<QLabel*>(label); //Change pointer type from QWidget to QLabel
                labelGood->setStyleSheet("background-color: lightsalmon;");
            }
        }
}
