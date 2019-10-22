#ifndef LASERTAB_H
#define LASERTAB_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>

class LaserTab : public QWidget
{
    Q_OBJECT
public:
    explicit LaserTab(QWidget *parent = nullptr);
    QPushButton *laserOn;
    QPushButton *laserOff;

signals:

public slots:
    void laserEventHandler();
    void laserEventIntensity(QString intensity);
};

#endif // LASERTAB_H
