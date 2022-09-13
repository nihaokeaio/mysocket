#ifndef _CELLTAST_H_
#define _CELLTAST_H_
#include<list>
#include<mutex>
#include<thread>

class CellTask{
private:
public:
    CellTask(){}
    virtual ~CellTask(){}
    virtual void doTask(){};
};

class CelltaskServer{
private:
    std::list<CellTask*>_tasks;
    std::list<CellTask*>_taskBuf;
    std::mutex _mutex;
public:
    CelltaskServer(){}
    ~CelltaskServer(){}
    void addtask(CellTask* task){
        std::lock_guard<std::mutex>lock(_mutex);
        _taskBuf.push_back(task);
    }
    //启动任务
    void Start(){
        std::thread t(&CelltaskServer::OnRun,this);
        t.detach();
    }

    void OnRun(){
        while(true){
            if(!_taskBuf.empty()){
            std::lock_guard<std::mutex>lock(_mutex);
            for(auto ptask:_taskBuf){
                _tasks.push_back(ptask);
            }
            _taskBuf.clear();
            }
        //如果没有任务
            if(_tasks.empty()){
                std::chrono::milliseconds t(1);
                std::this_thread::sleep_for(t);
                continue;
            }
            for(auto ptask:_taskBuf){
                ptask->doTask();
                delete ptask;
            }
            _tasks.clear(); 
        }
        
    }



};








#endif