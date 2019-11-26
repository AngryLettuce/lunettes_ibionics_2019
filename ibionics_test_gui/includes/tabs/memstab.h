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

class MemsTab : public QWidget
{
    Q_OBJECT
public:
    explicit MemsTab(QWidget *parent = nullptr, MainWindow* mW = nullptr);
    MediaLabel *lbl;
    QPixmap *pix;
    QLabel *posMouseLabel;
    QGridLayout *layout;

    MainWindow* mainWindowPtr;

signals:


public slots:
    void comboboxItemChanged(QString sequence);
};

#endif // MEMSTAB_H
