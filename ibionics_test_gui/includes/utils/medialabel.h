#ifndef MEDIALABEL_H
#define MEDIALABEL_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QObject>
#include <QDebug>
#include "memstab.h"

class MemsTab; //foward declaration to avoid circular dependencies with memstab.h

class MediaLabel : public QLabel
{
    Q_OBJECT

public:
    explicit MediaLabel(QWidget *parent = nullptr, QString filename = "");
    QPixmap* pix;
    MemsTab* memsTab;
    int posX = 0;
    int posY = 0;
signals:


public slots:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private :
    void pressManager(QMouseEvent *event);
};

#endif // MEDIALABEL_H
