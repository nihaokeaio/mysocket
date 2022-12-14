#ifndef _MYSERVER_H_
#define _MYSERVER_H_
#ifdef _WIN32
    #ifndef FD_SETSIZE
    #define FD_SETSIZE 4024
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
#include"CELLTimestamp.h"
#include"Messagehead.hpp"
#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 1024
#endif
#define _SER_THREAD_COUNT   4
#include<mutex>
#include<thread>
#include<atomic>
class ClientSock;
class Myeasyserver;


//处理客户端的Server
class Cellserver{
private:
    //真实客户队列
    std::vector<ClientSock*>_clients;
    //缓冲客户队列
    std::vector<ClientSock*>_clientsBuff;
    SOCKET _sock;
    char _szRecv[BUFFER_SIZE]={};
    std::mutex _mutex;
    std::thread* _pthread;
    
public:
    std::atomic_int _recvcount;    
    Cellserver(SOCKET sock=INVALID_SOCKET){
        _sock=sock;
        _pthread=nullptr;
        _recvcount=0;
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
    void addClient(ClientSock* pclient);
    void start();
    size_t getclientcount();
};

//对接客户端的SOCKET
class ClientSock{
private:
    SOCKET _sock;
    char _szMsgBuf[BUFFER_SIZE*10];
    int _lastPos;
public:
    ClientSock(SOCKET sock){
        _sock=sock;
        memset(_szMsgBuf,0,sizeof(_szMsgBuf));
        _lastPos=0;
    }
    SOCKET getsock(){
        return _sock;
    }
    char* Msgbuff(){
        return _szMsgBuf;
    }
    void setlastPos(int pos=0){
        _lastPos=pos;
    }
    int getlastPos(){
        return _lastPos;
    }
};


//监听服务器
class Myeasyserver{
private:
    SOCKET _sock;
    char _szRecv[BUFFER_SIZE]={};
    CELLTimestamp _curtime;
    std::vector<ClientSock*>g_clients;  
    std::vector<Cellserver*>_cellservers;
public:

    Myeasyserver(){
        _sock=INVALID_SOCKET;
    }
    virtual ~Myeasyserver(){
        Close();
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

    //相应网络消息
    virtual void OnNetMsg(SOCKET csock,DataHeader* header);
    //发送数据
    int SendData(SOCKET csock, DataHeader* header);
    void SendDataToAll(DataHeader* header);
    void Clear_client();

    void start();
    void AddclientToCellserver(ClientSock* pcllient);
    //打印接受信息
    void getMsgOnClient();
};







#endif