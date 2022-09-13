#include<iostream>
#include<thread>
#include"CELLTimestamp.h"
#include<mutex>
#include<atomic>
using namespace std;
atomic_int sum=0;
mutex mux;
void getsum(){
    
    for(int i=0;i<10000000;i++){//循环外加锁消耗<不加锁*线程个数<lock_guard<<使用原子变量<循环内加普通锁<循环内加lock_guard
        //lock_guard<mutex>lg(mux);
        //mux.lock();
        sum++;
        //mux.unlock();
    }
    
    return;
}

int tcount=10;
int main(){
    thread t[tcount];
    CELLTimestamp start;
    for(int i=0;i<tcount;i++){
        t[i]=thread(getsum);
    }
    for(int i=0;i<tcount;i++){
        t[i].join();
    }
    cout<<"sum="<<sum<<endl;
    cout<<"using time is "<<start.getElapsedsecondInMillSec()<<endl;
    start.update();
    sum=0;
    for(int i=0;i<100000000;i++){
        sum++;       
    }
    cout<<"using time is "<<start.getElapsedsecondInMillSec()<<endl;
    //system("pause");
    return 0;
}