#ifndef _CELLSERVER_H_
#define _CELLSERVER_H_
#include"myserver.h"
#include<mutex>
#include<thread>


class Cellserver{
public:
    Cellserver(SOCKET sock=INVALID_SOCKET){
        _sock=sock;
        _pthread=nullptr;
    }
    ~Cellserver(){
        Close();
        _sock=INVALID_SOCKET;
    }
    //关闭socket
    void Close();
    //处理网络消息
    bool OnRun();
    //是否在工作中
    bool isRun();
    //接收数据，处理战报，拆分包
    int RecvDate(ClientSock* csock);

    virtual void OnNetMsg(SOCKET csock,DataHeader* header);
    //发送数据
    int SendData(SOCKET csock, DataHeader* header);
    void SendDataToAll(DataHeader* header);
    void Clear_client();
    void addClient(ClientSock* pclient){
        std::lock_guard<std::mutex>lock(_mutex);
        _clientsBuff.push_back(pclient);
    }
    void start();
    ssize_t getclientcount();
private:
    //真实客户队列
    std::vector<ClientSock*>_clients;
    //缓冲客户队列
    std::vector<ClientSock*>_clientsBuff;
    SOCKET _sock;
    std::mutex _mutex;
    std::thread* _pthread;
};

#endif