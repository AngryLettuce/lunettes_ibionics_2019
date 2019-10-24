#ifndef EYETAB_H
#define EYETAB_H

#include <Qt>
#include <QTabWidget>
#include <QLayout>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>


class EyeTab : public QWidget
{
    Q_OBJECT
public:
    explicit EyeTab(QWidget *parent = nullptr);
    QCamera *camera;
    QCameraViewfinder *viewfinder;

signals:

public slots:

};

#endif // EYETAB_H