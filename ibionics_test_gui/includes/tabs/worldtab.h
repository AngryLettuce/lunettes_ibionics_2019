#ifndef WORLDTAB_H
#define WORLDTAB_H

#include <Qt>
#include <QSize>
#include <QWidget>
#include <QLayout>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>

class WorldTab : public QWidget
{
    Q_OBJECT
public:
    explicit WorldTab(QWidget *parent = nullptr);
    QCamera *camera;
    QCameraViewfinder *viewfinder;
signals:

public slots:

};

#endif // WORLDTAB_H
