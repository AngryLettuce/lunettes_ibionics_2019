#ifndef GPIOVIEW_H
#define GPIOVIEW_H

#include <QLabel>
#include <QHBoxLayout>

class GpioView : public QWidget
{
    Q_OBJECT
public:
    explicit GpioView(QString name = "NotDefined", QWidget *parent = nullptr);
    void setGpioName(QString name);

signals:

public slots:
    void updateIn(bool currentState);
    void updateOut(bool currentState);
    void inUse(bool state);

private :
    QLayout * gpioLayout;
    QLabel * gpioName;
    QLabel * inputBeacon;
    QLabel * outputBeacon;
    QSpacerItem * spacer ;
};

#endif // GPIOVIEW_H
