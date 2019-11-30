	#include "memstab.h"

MemsTab::MemsTab(QWidget *parent, MainWindow* mW) : QWidget(parent)
{
    //Layout
    layout = new QGridLayout(this);

    mainWindowPtr = mW;
        
    //PixMap
    //QString filename = "images\\eye.jpg";
    imgLblEye = new MediaLabel(this, "");
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
    layout->addWidget(imgLblEye,0,0);
    layout->addWidget(seqLbl,2,0,1,1);
    layout->addWidget(seqCombo,3,0,1,1);
    layout->addWidget(button,3,1,1,1);
    layout->addWidget(posMouseLabel,2,3,1,1);

    //Link signals to slots
    connect(seqCombo, SIGNAL(activated(int)), this, SLOT(comboboxItemChanged(int)));
    connect(button, SIGNAL (clicked()), this, SLOT (switchLaserState()));

    //mainWindowPtr = mW;
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

void MemsTab::processMemsFrame()
{
    imgEye = *(mainWindowPtr->cameras->readImgCam(0));
    if(imgEye.empty()) return;

    //Crop and resize EyeCam image according to calibration settings
    cropRegion(&imgEye, &imgEye, mainWindowPtr->calibrationPosX, mainWindowPtr->calibrationPosY, mainWindowPtr->roiSize, mainWindowPtr->roiSize, false);
    cv::resize(imgEye, imgEye, cv::Size(CAMERA_RESOLUTION-1, CAMERA_RESOLUTION-1), 0, 0, cv::INTER_LINEAR); // CAMERA_RESOLUTION - 1 ?
    posX = imgLblEye->posX;
    posY = imgLblEye->posY;
    saturateValue(posX, 0, CAMERA_RESOLUTION - 1);
    saturateValue(posY, 0, CAMERA_RESOLUTION - 1);
    if(lastposX != posX || lastposY != posY){

        mainWindowPtr->laser_pos_control.send_pos(posX, posY);
        lastposX = posX;
        lastposY = posY;
        //posMouseLabel->setText("X:%d,Y:%d", posX, posY); // TODO: code a function to update the text
    }
    
    cv::cvtColor(imgEye,imgEye,cv::COLOR_BGR2RGB);
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));

    //std::cout<<"PosX: "<< posX<<" PosY: "<< posY<<std::endl;
}
