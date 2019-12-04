#ifndef EYEWORLDTAB_H
#define EYEWORLDTAB_H

#include <iostream>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <opencv2/core.hpp>

#include "mainwindow.h"
#include "crop.h"
#include "edgeDetection.h"
#include "ellipsefit.h"
#include "houghCircle.h"
#include "modes.h"
#include "traitementworld.h"
#include "chrono"
#include "ctime"

#define BUTTON_METHOD_ROW 0
#define FPS_ROW 1
#define SLIDER_PARAM_ROW 2
#define IMAGES_ROW 3
#define COMBOBOX_INTERMEDIATE_ROW 4
#define SPACER_ROW 5


#define IMAGE_EYE_COLUMN 0
#define IMAGE_WORLD_COLUMN 1
#define SPACER_COLUMN 2

#define NORMAL_ROW_SPAN 1
#define NORMAL_COLUMN_SPAN 1

#define BUTTON_COLUMN_SPAN 2
#define SLIDER_COLUMN_SPAN 1
#define SPACER_ROW_SPAN 6
#define SPACER_COLUMN_SPAN 3

#define ROI_LINES 160
#define ROI_COL 180

#define LOWPASS_CUTOFF_VALUE 5

class MainWindow; //foward declaration to avoid circular dependencies with mainwindow.h
class Laser_pos_control; //foward declaration to avoid circular dependencies with mainwindow.h

extern std::vector<cv::Mat> frameBufferCam0 ;
extern std::vector<cv::Mat> frameBufferCam1 ;

class EyeWorldTab : public QWidget
{
    Q_OBJECT
public:
    explicit EyeWorldTab(QWidget *parent = nullptr, MainWindow* mW = nullptr);

    cv::Mat imgEye;
    cv::Mat imgEyeCropped;
    cv::Mat imgEyeResized;
    cv::Mat imgEyeToShow;

    cv::Mat imgWorld;
    cv::Mat imgWorldCropped;
    cv::Mat imgWorldOverlapped;
    cv::Mat imgWorldToShow;

    QLabel *imgLblEye;
    QLabel *imgLblWorld;

    QLabel *eyeFpsLabel;
    QLabel *worldFpsLabel;

    QPushButton *button_interpol;
    
    QPushButton *button_method;
    QSlider *slider_threshold;
    QSlider *slider_ROI;
    QGridLayout *layout;

    int posX = 0;
    int posY = 0;
    bool pupilMethod = true;
    int interpolMethod = 0;
    MainWindow* mainWindowPtr;

    cv::Point pupilPos;
    //cv::Mat processedImg;

    QComboBox stepsCombo;

    std::chrono::time_point<std::chrono::system_clock> startProcessEye, endProcessEye, startProcessWorld, endProcessWorld;

    cv::Mat gray_LUT;
    uchar*gray_level_LUT_pointer;

signals:

public slots:
    void processFrameEye();
    void processFrameWorld();
    void switchPupilMethodButton();
    void switchInterpolMethodButton();

};

#endif // EYEWORLDTAB_H
