#ifndef MEDIALABEL_H
#define MEDIALABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <iostream>

class MediaLabel : public QLabel
{
    Q_OBJECT

public:
    explicit MediaLabel(QWidget *parent = nullptr, QString filename = "");
    QPixmap* pix;
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
