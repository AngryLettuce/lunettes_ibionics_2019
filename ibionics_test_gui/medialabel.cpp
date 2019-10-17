#include "medialabel.h"


MediaLabel::MediaLabel(QWidget *parent, QString filename) : QLabel(parent)
{

    pix = new QPixmap(filename);
    this->setPixmap(*pix);
    memsTab = reinterpret_cast<MemsTab*>(parent); //Change pointer type from QWidget to MemsTab

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
    int x = pos.rx();
    int y = pos.ry();

    //Cap position values to 400x400
    if(x<0)
        x = 0;
    else if(x>400)
        x = 400;
    if(y<0)
        y = 0;
    else if(y>400)
        y = 400;

    QString currPos = QStringLiteral("X:%1, Y:%2").arg(x).arg(y);
    memsTab->posMouseLabel->setText(currPos);

}



