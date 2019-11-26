#ifndef THREADS_H
#define THREADS_H

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include "crop.h"
#include "edgeDetection.h"
#include "grayLevelsTable.h"
#include "ellipsefit.h"
#include "houghCircle.h"
#include "modes.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

//Global objects/flags for threads
extern std::mutex mx;

std::thread startEyeThread();
std::thread startWorldThread2();
std::thread startWorldThread(cv::Mat3b *img);
void stopEyeThread();
void stopWorldThread();
void WorldThread2(int id);
void WorldThread(int id, cv::Mat3b *img);
void EyeThread(int id);


#endif // THREADS_H
