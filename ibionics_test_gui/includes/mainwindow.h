#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

#include "memstab.h"
#include "lasertab.h"
#include "eyetab.h"
#include "worldtab.h"
#include "gpiotab.h"

class WorldTab; //foward declaration to avoid circular dependencies with worldtab.h
class EyeTab; //foward declaration to avoid circular dependencies with eyetab.h

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MemsTab *memsTab;
    LaserTab *laserTab;
    EyeTab *eyeCamTab;
    WorldTab *worldCamTab;
    GPIOTab *gpioTab;

    bool *hasCamera;

    int posX;
    int posY;

    int getPosX();
    int getPosY();
    void setPosX(int x);
    void setPosY(int y);

private:
    int memsIndex;
    int laserIndex;
    int eyeCamIndex;
    int worldCamIndex;

    QPixmap *qPixMapImage;

public slots:
    void tabChange(int currentIndex);
};
#endif // MAINWINDOW_H
