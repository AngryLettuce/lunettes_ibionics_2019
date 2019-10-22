#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

#include "includes/tabs/memstab.h"
#include "includes/tabs/lasertab.h"
#include "includes/tabs/eyetab.h"
#include "includes/tabs/worldtab.h"
#include "includes/tabs/gpiotab.h"

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
