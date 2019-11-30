#ifndef MEMSTAB_H
#define MEMSTAB_H

#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <QLabel>
#include <QPixmap>
#include <opencv2/core.hpp>
#include "medialabel.h"
#include "mainwindow.h"
#include "saturate.h"

class MainWindow; //foward declaration to avoid circular dependencies with mainwindow.h
//class Laser_pos_control; //foward declaration to avoid circular dependencies with eyeWorldtab.h

class MemsTab : public QWidget
{
    Q_OBJECT
public:
    explicit MemsTab(QWidget *parent = nullptr, MainWindow* mW = nullptr);
    MediaLabel *imgLblEye;
    cv::Mat imgEye;

    QPixmap *pix;
    QLabel *posMouseLabel;
    QPushButton *button;
    QGridLayout *layout;
    int posX, posY;
    int lastposX = 0;
    int lastposY = 0;
    char XYposText[6];

    bool laser_on = true;

    MainWindow* mainWindowPtr;

signals:


public slots:
    void processMemsFrame();
    void comboboxItemChanged(int index);
    void switchLaserState();
};

#endif // MEMSTAB_H
