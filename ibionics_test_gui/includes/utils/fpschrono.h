#ifndef FPSCHRONO_H
#define FPSCHRONO_H

#include <iostream>
#include <vector>
#include <chrono>
#include <time.h>


//template <typename T>
class FPSCHRONO
{
    //vecteur for store start time
    std::vector<int> detlaTime;
    clock_t t1;
    clock_t t2;
    double delta;
    //std::vector<long int> stopTime;

public:
    FPSCHRONO() {}

   void chronoStart(void);
   void chronoStop(void);
};


#endif // FPSCHRONO_H
