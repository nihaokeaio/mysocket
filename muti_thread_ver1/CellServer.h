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
    //�ر�socket
    void Close();
    //����������Ϣ
    bool OnRun();
    //�Ƿ��ڹ�����
    bool isRun();
    //�������ݣ�����ս������ְ�
    int RecvDate(ClientSock* csock);

    virtual void OnNetMsg(SOCKET csock,DataHeader* header);
    //��������
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
    //��ʵ�ͻ�����
    std::vector<ClientSock*>_clients;
    //����ͻ�����
    std::vector<ClientSock*>_clientsBuff;
    SOCKET _sock;
    std::mutex _mutex;
    std::thread* _pthread;
};

#endif