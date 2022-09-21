#ifndef _CELLTIMESTAMP_H_
#define _CELLTIMESTAMP_H_


//#include <Windows.h>
#include<chrono>
using namespace std::chrono;
class CELLTimestamp{
public:
    CELLTimestamp(){
        //QueryPerformanceCounter(&_startCount);
        //QueryPerformanceFrequency(&_frequency);
        update();
    }
    ~CELLTimestamp(){}
    void update(){
        //QueryPerformanceCounter(&_startCount);
        _begin=high_resolution_clock::now();
    }
    //get second
    double getElapsedsecond(){
        return this->getElapsedsecondInMicroSec()*0.000001;
    }
    //get millsec
    double getElapsedsecondInMillSec(){
        return this->getElapsedsecondInMicroSec()*0.001;
        
    }
    //get microsec
    long long getElapsedsecondInMicroSec(){
        /*
        LARGE_INTEGER endCount;
        QueryPerformanceCounter(&endCount);
        double startTimeInMicroSec=_startCount.QuadPart*(1000000.0/_frequency.QuadPart);
        double endTimeInMicroSec=endCount.QuadPart*(1000000.0/_frequency.QuadPart);
        return endTimeInMicroSec-startTimeInMicroSec;
        */
       return duration_cast<microseconds>(high_resolution_clock::now()-_begin).count();
    }
private:
    /*    //WINDOWS
    LARGE_INTEGER _frequency;
    LARGE_INTEGER _startCount;
    */
   //c++11 support
    time_point<high_resolution_clock>_begin;
};

#endif
