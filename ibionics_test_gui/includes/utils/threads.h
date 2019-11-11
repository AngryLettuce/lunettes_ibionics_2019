#ifndef THREADS_H
#define THREADS_H

//#include <cstdlib>
#include <iostream>
//#include <numeric>
//#include <sstream>
#include <thread>
#include <chrono>
#include <mutex>
#include "crop.h"
#include "edgeDetection.h"
#include "grayLevelsTable.h"
#include "ellipsefit.h"
#include "houghCircle.h"


extern std::mutex mx;

std::thread startEyeThread();
std::thread startWorldThread(cv::Mat3b *img);
void stopEyeThread();
void stopWorldThread();
void WorldThread(int id, cv::Mat3b *img);
void EyeThread(int id);


#endif // THREADS_H
