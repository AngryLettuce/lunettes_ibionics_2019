#ifndef MEMSTAB_H
#define MEMSTAB_H

#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <QLabel>
#include <QPixmap>
#include "medialabel.h"
#include "mainwindow.h"

class MainWindow; //foward declaration to avoid circular dependencies with mainwindow.h
//class Laser_pos_control; //foward declaration to avoid circular dependencies with eyeWorldtab.h

class MemsTab : public QWidget
{
    Q_OBJECT
public:
    explicit MemsTab(QWidget *parent = nullptr, MainWindow* mW = nullptr);
    MediaLabel *lbl;
    QPixmap *pix;
    QLabel *posMouseLabel;
    QPushButton *button;
    QGridLayout *layout;

    bool laser_on = true;

    MainWindow* mainWindowPtr;

signals:


public slots:
    void comboboxItemChanged(QString sequence);
    void switchLaserState();
};

#endif // MEMSTAB_H
