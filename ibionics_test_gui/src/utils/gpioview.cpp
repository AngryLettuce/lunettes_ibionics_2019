#include "gpioview.h"

GpioView::GpioView(QString name, QWidget *parent) : QWidget(parent)
{
    gpioLayout = new QHBoxLayout(this);

    gpioName = new QLabel("Not set");
    gpioName->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    gpioName->setText(name);

    inputBeacon = new QLabel(this);
    inputBeacon->setFixedSize(15, 15);
    inputBeacon->setAlignment(Qt::AlignCenter);

    outputBeacon = new QLabel(this);
    outputBeacon->setFixedSize(15, 15);
    outputBeacon->setAlignment(Qt::AlignCenter);

    gpioLayout->setMargin(0);
    gpioLayout->setContentsMargins(0,0,0,0);
    gpioLayout->addWidget(inputBeacon);
    gpioLayout->addWidget(outputBeacon);
    gpioLayout->addWidget(gpioName);
}

void GpioView::setGpioName(QString name)
{
    gpioName->setText(name);
}

void GpioView::updateIn(bool currentState)
{
    // Should be updated by events of the gpio on the RPi
    // TO BE IMPLEMENTED
    if (currentState)
        inputBeacon->setText("X");
    else
        inputBeacon->setText(" ");
}

void GpioView::updateOut(bool currentState)
{
    // Should send the real signal on the gpio with this function on the RPi
    // TO BE IMPLEMENTED
    if (currentState)
        outputBeacon->setText("X");
    else
        outputBeacon->setText(" ");
}

void GpioView::inUse(bool state)
{
    if (state)
        gpioName->setStyleSheet("background-color:springgreen;");
    else
        gpioName->setStyleSheet("background-color:lightsalmon;");
}
