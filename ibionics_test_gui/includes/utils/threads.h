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




std::thread startEyeThread();
std::thread startWorldThread();
void stopEyeThread();
void stopWorldThread();
void WorldThread(int id);
void EyeThread(int id);


#endif // THREADS_H
