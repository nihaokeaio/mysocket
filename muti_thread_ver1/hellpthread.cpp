#include<iostream>
#include<thread>
#include"CELLTimestamp.h"
#include<mutex>
//#include <stdlib.h>
#include"..//HelloMemory//Alloc.h"
using namespace std;
mutex mux;




const int tCount =12;
const int MCount = 1000000;
const int nCount = MCount/ tCount;

void getsum(){
    
    char* data[nCount];
    for (int i = 0; i < nCount; i++) {
        data[i] = new char[1 + (rand()%128)];

    }
    for (int i = 0; i < nCount; i++) {
        delete[] data[i];
    }    
    return;
}


int main(){
    thread t[tCount];
    CELLTimestamp start;
    for(int i=0;i< tCount;i++){
        t[i]=thread(getsum);
    }
    for(int i=0;i< tCount;i++){
        t[i].join();
    }
    cout<<"using time is "<<start.getElapsedsecondInMillSec()<<endl;
    /*start.update();
    for(int i=0;i<100000000;i++){
               
    }
    */
    cout<<"hello world"<<endl;
    //system("pause");
    return 0;
}