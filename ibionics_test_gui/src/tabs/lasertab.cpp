#include "includes/tabs/lasertab.h"

LaserTab::LaserTab(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);

    //Laser toggle
    QLabel *toggleLbl = new QLabel("Laser Toggle: ",this);

    laserOn = new QPushButton(this);
    laserOn->setText("Laser ON");

    laserOff = new QPushButton(this);
    laserOff->setText("Laser OFF");

    //Intensity
    QLabel *intenLbl = new QLabel("Intensity: ",this);
    QComboBox *intenCombo = new QComboBox();
    intenCombo->addItem("Low");
    intenCombo->addItem("Medium");
    intenCombo->addItem("High");

    //Placement in Layout
    QSpacerItem *spacerUp = new QSpacerItem(1,100);
    QSpacerItem *spacerDown = new QSpacerItem(1,100);
    layout->addItem(spacerUp,0,0);
    layout->addWidget(toggleLbl,1,0);
    layout->addWidget(laserOn,2,0);
    layout->addWidget(laserOff,2,1);
    layout->addWidget(intenLbl,3,0);
    layout->addWidget(intenCombo,4,0);
    layout->addItem(spacerDown,5,0);

    //Link signals to slots
    connect(laserOn, SIGNAL(released()), this, SLOT(laserEventHandler()));
    connect(laserOff, SIGNAL(released()), this, SLOT(laserEventHandler()));
    connect(intenCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(laserEventIntensity(QString)));
}

void LaserTab::laserEventHandler()
{
    //qDebug("From Laser Tab");
    QPushButton *releasedBtn = qobject_cast<QPushButton*>(sender());
    if(releasedBtn->text() == "Laser ON")
        qDebug("Laser Turned ON");

    else if(releasedBtn->text() == "Laser OFF")
        qDebug("Laser Turned OFF");

}

void LaserTab::laserEventIntensity(QString intensity)
{
    qDebug() << intensity;
}
