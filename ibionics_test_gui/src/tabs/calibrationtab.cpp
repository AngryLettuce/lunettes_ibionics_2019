#include "calibrationtab.h"

#define VCOS_ALIGN_DOWN(p,n) (((ptrdiff_t)(p)) & ~((n)-1))
#define VCOS_ALIGN_UP(p,n) VCOS_ALIGN_DOWN((ptrdiff_t)(p)+(n)-1,(n))

CalibrationTab::CalibrationTab(QWidget *parent, MainWindow* mW) : QWidget(parent)
{
    //TODO Init size of config in init file
    rows = Y_ANGLES_GRID_POINTS;
    columns = X_ANGLES_GRID_POINTS;
    
    mainWindowPtr = mW;
    
    layout = new QGridLayout(this);
    imgLblEye = new MediaLabel(this, "");
    button = new QPushButton("Start Calibration", this);
    QSpacerItem *lastColSpacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem *lastRowSpacer = new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding);


    slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(100);
    slider->setMaximum(400);
    slider->setValue(mainWindowPtr->roiSize);
    
    layout->addWidget(imgLblEye,1,0);
    layout->addWidget(button,0,0);
    layout->addWidget(slider,2,0);

    layout->addItem(lastColSpacer,0,1,3,1);
    layout->addItem(lastRowSpacer,3,0,1,1);

    connect(button, SIGNAL (clicked()), this, SLOT (startCalibration()));
    
    QWidget::setFocusPolicy(Qt::StrongFocus);
    
    imgLblEye->posX =  mainWindowPtr-> calibrationPosX;
    imgLblEye->posY =  mainWindowPtr-> calibrationPosY;
}

void CalibrationTab::processCalibrationFrame()
{

    imgEye = *mainWindowPtr->cameras->readImgCam(0);
    if(imgEye.empty()) return;
    
    //Get corners of roi
    if(!inCalibration){
        roiSize = slider->value();
        upLeft = cv::Point(imgLblEye->posX  - roiSize/2, imgLblEye->posY  - roiSize/2);
        downRight = cv::Point(imgLblEye->posX  + roiSize/2, imgLblEye->posY   + roiSize/2);
    }

    //Add grid to roi
    int leftSide = upLeft.x; 
    int upSide = upLeft.y;
    
    int rightSide = downRight.x;
    int downSide = downRight.y;
    
    int heightSpace = (rightSide - leftSide)/(rows-1);
    int widthSpace = (downSide - upSide)/(columns-1);
    
    // Drawing calibration grid
    cv::cvtColor(imgEye,imgEye,cv::COLOR_BGR2RGB);
    int line_pos, i;
    for (i = 0, line_pos = upSide; i < rows-1; i++, line_pos+=heightSpace)
    
        cv::line(imgEye, cv::Point(leftSide, line_pos), cv::Point(rightSide, line_pos), cv::Scalar(0, 255, 0));

    for (i = 0, line_pos = leftSide; i < columns-1; i++, line_pos+=widthSpace)
        cv::line(imgEye, cv::Point(line_pos, upSide), cv::Point(line_pos, downSide), cv::Scalar(0, 255, 0));

    cv::rectangle(imgEye, cv::Rect(upLeft, downRight) , cv::Scalar(0,255,0), 1, 8,0 );
    QImage qimgEye(reinterpret_cast<uchar*>(imgEye.data), imgEye.cols, imgEye.rows, imgEye.step, QImage::Format_RGB888);
    imgLblEye->setPixmap(QPixmap::fromImage(qimgEye));   
}

void CalibrationTab::keyPressEvent(QKeyEvent *event)
{
    if(inCalibration)
    {
        #ifdef __arm__
        currentKey = event->key();
        
        if (lastKeyPressed != currentKey)
            momentum = delta_angle;

        switch(currentKey){
        case Qt::Key_W:
            angle_x += momentum;
            //std::cout <<  angle_x << std::endl;
            angle_x = mainWindowPtr->laser_pos_control.mems.send_angle_x(angle_x);
            momentum += delta_angle;
            break;
        case Qt::Key_S:
            angle_x -= momentum;
            //std::cout <<  angle_x << std::endl;
            angle_x = mainWindowPtr->laser_pos_control.mems.send_angle_x(angle_x);
            momentum += delta_angle;
            break;
        case Qt::Key_A:
            angle_y -= momentum;
            //std::cout <<  angle_y << std::endl;
            angle_y = mainWindowPtr->laser_pos_control.mems.send_angle_y(angle_y);
            momentum += delta_angle;
            break;
        case Qt::Key_D:
            angle_y += momentum;
            //std::cout <<  angle_y << std::endl;
            angle_y = mainWindowPtr->laser_pos_control.mems.send_angle_y(angle_y);
            momentum += delta_angle;
            break;
        case Qt::Key_Space:
            
            mainWindowPtr->laser_pos_control.gridPointsX.at(row_calib_counter,column_calib_counter) = (double)mainWindowPtr->laser_pos_control.mems.get_angle_x();
            mainWindowPtr->laser_pos_control.gridPointsY.at(row_calib_counter,column_calib_counter) = (double)mainWindowPtr->laser_pos_control.mems.get_angle_y();
            
            //std::cout<<"GridPointsX :"<< std::endl;
            //mainWindowPtr->laser_pos_control->gridPointsX.print();
            //std::cout<<"GridPointsY :"<< std::endl;
            //mainWindowPtr->laser_pos_control->gridPointsY.print();
            std::cout<<"In Row: Point "<<row_calib_counter+1<<" out of "<< Y_ANGLES_GRID_POINTS << " done ";
            std::cout<<"for column "<<column_calib_counter+1<<" out of "<< X_ANGLES_GRID_POINTS <<std::endl;
            mainWindowPtr->laser_pos_control.mems.print_angles();
            
            if(column_calib_counter < X_ANGLES_GRID_POINTS - 1)
                column_calib_counter++;
            else
            {
                column_calib_counter = 0;
                row_calib_counter++;
                if(row_calib_counter >= Y_ANGLES_GRID_POINTS)
                {
                    std::cout<<"GridPointsX :"<< std::endl;
                    mainWindowPtr->laser_pos_control.gridPointsX.print();
                    std::cout<<"GridPointsY :"<< std::endl;
                    mainWindowPtr->laser_pos_control.gridPointsY.print();
                    row_calib_counter = 0;
                    mainWindowPtr->laser_pos_control.saveAnglePoints();
                    mainWindowPtr->laser_pos_control.initAngleMat();
                    
                    mainWindowPtr->calibrationPosX = imgLblEye->posX;
                    mainWindowPtr->calibrationPosY = imgLblEye->posY;
                    mainWindowPtr->roiSize = slider->value();
                    mainWindowPtr->saveCalibrationGridParams();
                    
                    mainWindowPtr->laser_pos_control.draw_rectangle();
                    
                    startCalibration();
                }     
            }
            
            
            break;
        default:
            break;
        }
        
        lastKeyPressed = currentKey;
        #endif
    }
}

void CalibrationTab::startCalibration()
{
     //get initial mems values
    angle_x = mainWindowPtr->laser_pos_control.mems.get_angle_x();
    angle_y = mainWindowPtr->laser_pos_control.mems.get_angle_y();
    
    row_calib_counter = 0;
    column_calib_counter = 0;
    momentum = 0;
    
    if(!inCalibration)
    {
        imgLblEye->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        slider->setEnabled(false);
        button->setText("In Calibration");
        inCalibration = true;
    }
    else
    {
        imgLblEye->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        slider->setEnabled(true);
        button->setText("Start Calibration");

        inCalibration = false;        
    }

    imgLblEye->setFocus();
}

