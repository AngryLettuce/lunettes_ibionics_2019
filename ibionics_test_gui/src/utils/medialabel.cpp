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
    //int posX = pos.rx();
    //int posY = pos.ry();
    posX = pos.rx();
    posY = pos.ry();

    //Cap position values to 400x400
    if(posX<0)
        posX = 0;
    else if(posX>400)
        posX = 400;
    if(posY<0)
        posY = 0;
    else if(posY>400)
        posY = 400;
    
    std::cout<<"X: "<<posX<< "Y: "<<posY<<std::endl;
    /*
    QString currPos = QStringLiteral("X:%1, Y:%2").arg(posX).arg(posY);
    memsTab->posMouseLabel->setText(currPos);
    */
}



