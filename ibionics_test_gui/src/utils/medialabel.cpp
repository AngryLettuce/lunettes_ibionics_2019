#include "medialabel.h"

MediaLabel::MediaLabel(QWidget *parent, QString filename) : QLabel(parent)
{
    pix = new QPixmap(filename);
    this->setPixmap(*pix);
    
}

void MediaLabel::mouseMoveEvent(QMouseEvent *event)
{
    pressManager(event);
}

void MediaLabel::mousePressEvent(QMouseEvent *event)
{
    pressManager(event);
}

void MediaLabel::pressManager(QMouseEvent *event){
    QPoint pos = event->pos();
    posX = pos.rx();
    posY = pos.ry();

    /*
    QString currPos = QStringLiteral("X:%1, Y:%2").arg(posX).arg(posY);
    memsTab->posMouseLabel->setText(currPos);
    */
}



