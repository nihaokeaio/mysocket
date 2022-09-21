#ifndef _CELLTAST_H_
#define _CELLTAST_H_
#include "myserver.h"


class CellTask
{
private:
public:
    CellTask() {}
    virtual ~CellTask() {}
    virtual void doTask(){};
};


typedef std::shared_ptr<CellTask> CellTask_SMP;

class CelltaskServer
{
private:
    std::list<CellTask_SMP> _tasks;
    std::list<CellTask_SMP> _taskBuf;
    std::mutex _mutex;

public:
    CelltaskServer() {}
    virtual ~CelltaskServer() {}
    void addtask(CellTask_SMP& task)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _taskBuf.push_back(task);
    }
    //启动任务
    void Start()
    {
        std::thread t(&CelltaskServer::OnRun, this);
        t.detach();
    }

    void OnRun()
    {
        while (true)
        {
            if (!_taskBuf.empty())
            {
                std::lock_guard<std::mutex> lock(_mutex);
                for (auto ptask : _taskBuf)
                {
                    _tasks.push_back(ptask);
                }
                _taskBuf.clear();
            }
            //如果没有任务
            if (_tasks.empty())
            {
                std::chrono::milliseconds t(1);
                std::this_thread::sleep_for(t);
                continue;
            }
            for (auto ptask : _tasks)
            {
                ptask->doTask();
            }
            _tasks.clear();
        }
    }
};

#endif