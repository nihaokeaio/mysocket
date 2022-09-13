#ifndef _MYSERVER_H_
#define _MYSERVER_H_
#ifdef _WIN32
    #ifndef FD_SETSIZE
    #define FD_SETSIZE 2506
    #endif
    #define WIN32_LEAN_AND_MEAN
    #include<windows.h>
    #include<WinSock2.h>
    #pragma comment(lib,"ws2_32.lib")
#else
    #include<unstd.h>
    #include<arpa/inet.h>
    #include<string.h>

    #define SOCKET int
    #define INVALID_SOCKET (SOCKET)(~0)
    #define SOCKET_ERROR           (-1)
#endif


#include<stdio.h>
#include<vector>
#include<set>
#include"CELLTimestamp.h"
#include"Messagehead.hpp"
#include"CellTask.hpp"
#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 1024
#endif
#define _SER_THREAD_COUNT   4
#include<mutex>
#include<thread>
#include<atomic>

class ClientSock;
class Myeasyserver;
class Cellserver;


class Inetclient{
public:
    virtual void ClientleaveOn(ClientSock* pclient)=0;
    virtual void Msgcount()=0;
    virtual void Recvcount()=0;
};

//网络消息发送任务



//处理客户端的Server
class Cellserver{
private:
    //真实客户队列
    std::vector<ClientSock*>_clients;
    //缓冲客户队列
    std::vector<ClientSock*>_clientsBuff;
    SOCKET _sock;
    char* _szRecv;
    bool cmdOnRun;
    std::mutex _mutex;
    std::thread _pthread;
    Inetclient* _pclientleave;
    CelltaskServer _taskserver;
public:
       
    Cellserver(SOCKET sock=INVALID_SOCKET, Inetclient* plient=nullptr){
        _sock=sock;
        cmdOnRun = true;
        _pclientleave = plient;
        _szRecv = new char[BUFFER_SIZE];
    }
    ~Cellserver(){
        cmdOnRun = false;
        while (OnRun());
        Close();
        printf("Cellserver  is exit !\n");
        delete[] _szRecv;
        _szRecv = nullptr;
    }
    //关闭socket
    void Close();
    //处理网络消息
    bool OnRun();
    //是否在工作中
    bool isRun();
    //接收数据，处理战报，拆分包
    int RecvDate(ClientSock* csock);

    virtual void OnNetMsg(ClientSock*,DataHeader* header);
    //发送数据
    int SendData(ClientSock* csock, DataHeader* header);
    void SendDataToAll(DataHeader* header);
    void Clear_client();
    void addClient(ClientSock* pclient);
    void start();
    size_t getclientcount();
    void OnMsgToleave(ClientSock* plient);
    void OnMsgCount();
    void OnRecvCount();
    void addSendTask(CellTask* task);
};

//对接客户端的SOCKET
class ClientSock{
private:
    char* _szMsgBuf;
    SOCKET _sock;   
    int _lastPos;
public:
    ClientSock(SOCKET sock){  
        _szMsgBuf=new char[BUFFER_SIZE * 10];      
        _sock=sock;
        memset(_szMsgBuf,0,sizeof(_szMsgBuf));
        _lastPos=0;
    }
    ~ClientSock(){
        delete[] _szMsgBuf;
        printf("ClientSock _szMsgbuff is pop\n");
        Close();
    }
    SOCKET getsock(){
        return _sock;
    }
    char* Msgbuff(){
        return _szMsgBuf;
    }
    void SendData(DataHeader* header){
        if(header){
            if(send(_sock,(const char*)header,header->datalength,0)<0){
                printf("Socket <%d> send fail \n",_sock);
            }
        }
    }
    void setlastPos(int pos=0){
        _lastPos=pos;
    }
    int getlastPos(){
        return _lastPos;
    }
    void Close(){
        if(_sock!=INVALID_SOCKET){
    #ifdef _WIN32
        closesocket(_sock);
        //清楚windows socket环境
    #else
        Close();
    #endif
        _sock=INVALID_SOCKET;
        }
    }
};


//监听服务器
class Myeasyserver:public Inetclient
{
private:
    SOCKET _sock;
    std::atomic_int _msgcount;
    std::atomic_int _recvcount;
    char _szRecv[BUFFER_SIZE];
    CELLTimestamp _curtime;
    std::set<ClientSock*>g_clients;  
    std::vector<Cellserver*>_cellservers;
public:

    Myeasyserver(){
        _msgcount=0;
        _recvcount=0;
        _sock=INVALID_SOCKET;
        memset(_szRecv, 0, sizeof(_szRecv));
    }
    virtual ~Myeasyserver(){
        //printf("~mec run\n");
        Close();
        //delete _szRecv;
    }
    //初始化socket
    void InitSocket();
    //绑定端口号
    SOCKET  Bind(const char* ip,unsigned short port);
    //监听端口号
    int Listen(int n);
    //开始接受客户端连接
    int Accept();
    //关闭socket
    void Close();
    //处理网络消息
    bool OnRun();
    //是否在工作中
    bool isRun();
    //接收数据，处理战报，拆分包
    int RecvDate(ClientSock* csock);

    void start();
    void AddclientToCellserver(ClientSock* pcllient);
    //打印接受信息
    void getMsgOnClient();
    void ClientleaveOn(ClientSock* pclient);
    void Msgcount();
    void Recvcount();
};

class CellSendMsg2clientTask:public CellTask
{
private:
    ClientSock* _pClient;
    DataHeader* _pHeader;
public:
    CellSendMsg2clientTask(ClientSock* pClient,DataHeader* pHeader){
        _pClient=pClient;
        _pHeader=pHeader;
    }
    virtual void doTask(){
        _pClient->SendData(_pHeader);
        delete _pHeader;
    }

};





#endif