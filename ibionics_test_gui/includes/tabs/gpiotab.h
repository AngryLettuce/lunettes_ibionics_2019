#ifndef GPIOTAB_H
#define GPIOTAB_H

#include "gpioview.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class GPIOTab : public QWidget
{
    Q_OBJECT
public:
    explicit GPIOTab(QWidget *parent = nullptr);
    ~GPIOTab();
    void changeAllLabelsToRed();

    QWidget *mems;
    QWidget *laser;
    QWidget *pot;
    QWidget *camera;
    QWidget *aiy;

    QVBoxLayout *layoutMems;
    QVBoxLayout *layoutLaser;
    QVBoxLayout *layoutPot;
    QVBoxLayout *layoutCamera;
    QVBoxLayout *layoutAiy;

    QLabel *memsLbl;
    QLabel *laserLbl;
    QLabel *potLbl;
    QLabel *cameraLbl;
    QLabel *aiyLbl;

    GpioView *spi2;
    GpioView *memsEnable;

    GpioView *laserEnable;
    GpioView *laserEnCal;
    GpioView *laserEnChannelLow;
    GpioView *laserEnChannelMid;
    GpioView *laserEnChannelHigh;
    GpioView *laserRegen;

    GpioView *i2c0;

    GpioView *i2c1;

    GpioView *i2c0Aiy;
    GpioView *spi2Aiy;

signals:

public slots:
};

#endif // GPIOTAB_H
