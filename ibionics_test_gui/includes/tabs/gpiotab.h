#ifndef GPIOTAB_H
#define GPIOTAB_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>
#include "gpioview.h"

class GPIOTab : public QWidget
{
    Q_OBJECT
public:
    explicit GPIOTab(QWidget *parent = nullptr);
    ~GPIOTab();
    void changeAllLabelsToRed();

    //Widgets
    QWidget *mems;
    QWidget *laser;
    QWidget *pot;
    QWidget *camera;
    QWidget *aiy;

    //mems
    GpioView *spi2;
    GpioView *memsEnable;

    //Laser
    GpioView *laserEnable;
    GpioView *laserEnCal;
    GpioView *laserEnChannelLow;
    GpioView *laserEnChannelMid;
    GpioView *laserEnChannelHigh;
    GpioView *laserRegen;

    //digital potentiometer
    GpioView *i2c0;

    //Camera
    GpioView *i2c1;

    //AIY
    GpioView *i2c0Aiy;
    GpioView *spi2Aiy;

signals:

public slots:
};

#endif // GPIOTAB_H
