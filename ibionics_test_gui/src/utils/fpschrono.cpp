#include "fpschrono.h"


//class for chrono
//feature to do
//chrono start
//chrono stop
//strore time

void FPSCHRONO::chronoStart(void)
{
    //auto start = std::chrono::steady_clock::now();
    //t1 = std::chrono::steady_clock::now();
    t1 = clock();
}

void FPSCHRONO::chronoStop(void)
{
    //t2=  std::chrono::steady_clock::now();
    t2 = clock();
    delta = ((double)t2-t1)/CLOCKS_PER_SEC;
    std::cout << "temps ecouter : " << delta << "sec" << std::endl;
}
