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
    seqCombo = new QComboBox();
    QSpacerItem *lastColSpacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem *lastRowSpacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);

    seqCombo->addItem("Rectangle");
    seqCombo->addItem("Closing Rectangle");
    seqCombo->addItem("Spiral");
    seqCombo->addItem("Infinite");
    seqCombo->addItem("Circular Loop");

    button = new QPushButton("Stop Laser", this);

    //Placement in layout
    layout->addWidget(imgLblEye,0,0,1,1);
    layout->addWidget(seqLbl,1,0,1,1);
    layout->addWidget(seqCombo,2,0,1,1);
    layout->addWidget(button,2,1,1,1);
    layout->addWidget(posMouseLabel,1,1,1,1);

    layout->addItem(lastColSpacer,0,2,3,1);
    layout->addItem(lastRowSpacer,3,0,1,2);

    // allign the pixmap
    layout->setAlignment(imgLblEye, Qt::AlignLeft);
    layout->setAlignment(imgLblEye, Qt::AlignTop);


    //Link signals to slots
    connect(seqCombo, SIGNAL(activated(int)), this, SLOT(comboboxItemChanged(int)));
    connect(button, SIGNAL (clicked()), this, SLOT (switchLaserState()));
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
#ifdef __arm__
    (laser_on? mainWindowPtr->laser_pos_control.laser.off() : mainWindowPtr->laser_pos_control.laser.on());
#endif
    button->setText(laser_on? "Start Laser" : "Stop Laser");
    laser_on = !laser_on; //change stage of laser

    if (laser_on){
        seqCombo->setEnabled(true);
        connect(seqCombo, SIGNAL(activated(int)), this, SLOT(comboboxItemChanged(int)));
    }
    else{
        seqCombo->setEnabled(false);
        disconnect(seqCombo, SIGNAL(activated(int)), this, SLOT(comboboxItemChanged(int)));
    }
}

void MemsTab::processMemsFrame()
{
    if(frameBufferCam0.size() >= 2)
        imgEye = frameBufferCam0.back();
    if(imgEye.empty()) return;

    //Crop and resize EyeCam image according to calibration settings
    cropRegion(&imgEye, &imgEyeCropped, mainWindowPtr->calibrationPosX, mainWindowPtr->calibrationPosY, mainWindowPtr->roiSize, mainWindowPtr->roiSize, false);
    cv::resize(imgEyeCropped, imgEyeResized, cv::Size(CAMERA_RESOLUTION, CAMERA_RESOLUTION), 0, 0, cv::INTER_LINEAR);
    if(laser_on){
        posX = imgLblEye->posX;
        posY = imgLblEye->posY;
        saturateValue(posX, 0, CAMERA_RESOLUTION - 1);
        saturateValue(posY, 0, CAMERA_RESOLUTION - 1);
        if(lastposX != posX || lastposY != posY){
            mainWindowPtr->laser_pos_control.send_pos(posX, posY);
            updatePosMouseLabel(posX, posY);
            lastposX = posX;
            lastposY = posY;
        }
    }
    cv::cvtColor(imgEyeResized,imgEyeToShow,cv::COLOR_BGR2RGB);
    QImage qimgEye(reinterpret_cast<uchar*>(imgEyeToShow.data), imgEyeToShow.cols, imgEyeToShow.rows, imgEyeToShow.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));

    //std::cout<<"PosX: "<< posX<<" PosY: "<< posY<<std::endl;
}


void MemsTab::updatePosMouseLabel(int x, int y){
    char labelText[7];
    std::sprintf(labelText, "X:%d,Y:%d", x,y);
    posMouseLabel->setText(labelText);
}
