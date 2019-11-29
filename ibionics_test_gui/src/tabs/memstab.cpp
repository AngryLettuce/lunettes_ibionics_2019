#include "memstab.h"

MemsTab::MemsTab(QWidget *parent, MainWindow* mW) : QWidget(parent)
{
    //Layout
    layout = new QGridLayout(this);

    mainWindowPtr = mW;
        
    //PixMap
    QString filename = "images\\eye.jpg";
    lbl = new MediaLabel(this, filename);
    posMouseLabel = new QLabel(this);
    posMouseLabel->setText("X:0,Y0");

    //Sequence
    QLabel *seqLbl = new QLabel("Sequence: ",this);
    QComboBox *seqCombo = new QComboBox();
    seqCombo->addItem("Rectangle");
    seqCombo->addItem("Closing Rectangle");
    seqCombo->addItem("Spiral");
    seqCombo->addItem("Infinite");
    seqCombo->addItem("Circular Loop");

    button = new QPushButton("Stop Laser", this);

    //Placement in layout
    layout->addWidget(lbl,0,0,1,4);
    layout->addWidget(seqLbl,2,0,1,1);
    layout->addWidget(seqCombo,3,0,1,1);
    layout->addWidget(button,3,1,1,1);
    layout->addWidget(posMouseLabel,2,3,1,1);

    //Link signals to slots
    connect(seqCombo, SIGNAL(activated(int)), this, SLOT(comboboxItemChanged(int)));
    connect(button, SIGNAL (clicked()), this, SLOT (switchLaserState()));

    mainWindowPtr = mW;
}

void MemsTab::comboboxItemChanged(int index)
{
    switch(index) {
        case 0:
            mainWindowPtr->laser_pos_control.draw_rectangle(10);
            break;
        case 1:
            mainWindowPtr->laser_pos_control.draw_closing_rectangle(10);
            break;
        case 2:
            mainWindowPtr->laser_pos_control.draw_spiral(10);
            break;
        case 3:
            mainWindowPtr->laser_pos_control.draw_infinity(10);
            break;
        case 4:
            mainWindowPtr->laser_pos_control.draw_circular_loop(10);
            break;
    }
}

void MemsTab::switchLaserState()
{   
    (laser_on? mainWindowPtr->laser_pos_control.laser.off() : mainWindowPtr->laser_pos_control.laser.on());
    button->setText(laser_on? "Start Laser" : "Stop Laser");
    laser_on = !laser_on; //change stage of laser
}
