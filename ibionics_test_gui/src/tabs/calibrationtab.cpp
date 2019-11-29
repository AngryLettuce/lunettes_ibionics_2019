#include <fstream>

#include "calibrationtab.h"

#define CALIBRATION_GRID_PARAMS_FILENAME "calibrationGridParams.txt"
#define VCOS_ALIGN_DOWN(p,n) (((ptrdiff_t)(p)) & ~((n)-1))
#define VCOS_ALIGN_UP(p,n) VCOS_ALIGN_DOWN((ptrdiff_t)(p)+(n)-1,(n))

CalibrationTab::CalibrationTab(QWidget *parent, MainWindow* mW) : QWidget(parent)
{
    //TODO Init size of config in init file
    rows = Y_ANGLES_GRID_POINTS;
    columns = X_ANGLES_GRID_POINTS;
    
    layout = new QGridLayout(this);
    imgLblEye = new MediaLabel(this, "");
    button = new QPushButton("Start Calibration", this);

    slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(100);
    slider->setMaximum(400);
    slider->setValue(400);

    layout->addWidget(imgLblEye,1,0);
    layout->addWidget(button,0,0);
    layout->addWidget(slider,2,0);

    connect(button, SIGNAL (clicked()), this, SLOT (startCalibration()));
    connect(slider, SIGNAL (valueChanged(int)), this, SLOT (changeRoiSize(int)));
    mainWindowPtr = mW;
    
    QWidget::setFocusPolicy(Qt::StrongFocus);
    
    loadCalibrationGridParams();
    
}

void CalibrationTab::processCalibrationFrame()
{
    imgEye = *mainWindowPtr->cameras->readImgCam(0);
    if(imgEye.empty()) return;
    
    //Get corners of roi
    mainWindowPtr->upLeft = cv::Point(imgLblEye->posX - calibrationRoiSize/2,imgLblEye->posY - calibrationRoiSize/2);
    mainWindowPtr->downRight = cv::Point(imgLblEye->posX + calibrationRoiSize/2,imgLblEye->posY + calibrationRoiSize/2);

    //Add grid to roi
    mainWindowPtr->leftSide = mainWindowPtr->upLeft.x;
    mainWindowPtr->upSide = mainWindowPtr->upLeft.y;
    
    mainWindowPtr->rightSide = mainWindowPtr->downRight.x;
    mainWindowPtr->downSide = mainWindowPtr->downRight.y;
    
    int heightSpace = (mainWindowPtr->rightSide - mainWindowPtr->leftSide)/(rows-1);
    int widthSpace = (mainWindowPtr->downSide - mainWindowPtr->upSide)/(columns-1);
    
    // Drawing calibration grid
    for (int i = mainWindowPtr->upSide; i<mainWindowPtr->downSide; i += heightSpace)
        cv::line(imgEye, cv::Point(mainWindowPtr->leftSide, i), cv::Point(mainWindowPtr->rightSide, i), cv::Scalar(0, 255, 255));

    for (int i = mainWindowPtr->leftSide; i<mainWindowPtr->rightSide; i += widthSpace)
        cv::line(imgEye, cv::Point(i, mainWindowPtr->upSide), cv::Point(i, mainWindowPtr->downSide), cv::Scalar(255, 0, 255));
    
    cv::cvtColor(imgEye,imgEye,cv::COLOR_BGR2RGB);
    cv::rectangle(imgEye,cv::Rect(mainWindowPtr->upLeft,mainWindowPtr->downRight) , cv::Scalar(0,255,0), 1, 8,0 );
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
                    
                    saveCalibrationGridParams();
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
    mainWindowPtr->roiSize = slider->value();
    mainWindowPtr->calibrationPosX = mainWindowPtr->upLeft.x + mainWindowPtr->roiSize/2;
    mainWindowPtr->calibrationPosY = mainWindowPtr->upLeft.y + mainWindowPtr->roiSize/2;
    
     //get initial mems values
    angle_x = mainWindowPtr->laser_pos_control.mems.get_angle_x();
    angle_y = mainWindowPtr->laser_pos_control.mems.get_angle_y();
    
    row_calib_counter = 0;
    column_calib_counter = 0;
    momentum = 0;
    
    if(!inCalibration)
    {
        button->setText("In Calibration");
        inCalibration = true;
    }
    else
    {
        button->setText("Start Calibration");

        inCalibration = false;        
    }
}

void CalibrationTab::changeRoiSize(int size)
{
    calibrationRoiSize = size;
}

cv::Mat* CalibrationTab::getImage(int camNumber, int width, int height)
{

    IMAGE_FORMAT fmt = {IMAGE_ENCODING_I420, 100};
    BUFFER *buffer = nullptr;
#ifdef __arm__
    if(camNumber == 0)
        buffer = arducam_capture(mainWindowPtr->arducamInstance0, &fmt, 3000);
    else
        buffer = arducam_capture(mainWindowPtr->arducamInstance1, &fmt, 3000);
#endif
    if (!buffer)
        return nullptr;
#ifdef __arm__
    // The actual width and height of the IMAGE_ENCODING_RAW_BAYER format and the IMAGE_ENCODING_I420 format are aligned,
    // width 32 bytes aligned, and height 16 byte aligned.
    width = VCOS_ALIGN_UP(width, 32);
    height = VCOS_ALIGN_UP(height, 16);
    processedImg = cv::Mat(cv::Size(width,(int)(height * 1.5)), CV_8UC1, buffer->data);
    cv::cvtColor(processedImg, processedImg, cv::COLOR_YUV2BGR_I420);
    arducam_release_buffer(buffer);
    return &processedImg;
#endif
    return nullptr;
}

void CalibrationTab::saveCalibrationGridParams() {
    std::ofstream myfile(CALIBRATION_GRID_PARAMS_FILENAME);
    if(myfile.fail()) {
    }
    else {
        myfile << imgLblEye->posX << std::endl;
        myfile << imgLblEye->posY << std::endl;
        myfile << calibrationRoiSize << std::endl;
    }
}


void CalibrationTab::loadCalibrationGridParams() {
    std::ifstream myfile(CALIBRATION_GRID_PARAMS_FILENAME);
    if(myfile.fail()) {
        imgLblEye->posX = 0;
        imgLblEye->posY = 0;
        calibrationRoiSize = 400;
    }
    else {
        myfile >> imgLblEye->posX;
        myfile >> imgLblEye->posY;
        myfile >> calibrationRoiSize;
    }
}

