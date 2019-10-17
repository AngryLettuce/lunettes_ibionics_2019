#ifndef GPIOTAB_H
#define GPIOTAB_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QSpacerItem>

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
    QLabel *spi2;
    QLabel *memsEnable;

    //Laser
    QLabel *laserEnable;
    QLabel *laserEnCal;
    QLabel *laserEnChannelLow;
    QLabel *laserEnChannelMid;
    QLabel *laserEnChannelHigh;
    QLabel *laserRegen;

    //digital potentiometer
    QLabel *i2c0;

    //Camera
    QLabel *i2c1;

    //AIY
    QLabel *i2c0Aiy;
    QLabel *spi2Aiy;


signals:

public slots:
};

#endif // GPIOTAB_H
