#ifndef _MYSERVER_H_
#define _MYSERVER_H_
#ifdef _WIN32
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
#include"Messagehead.hpp"
#define BUFFER_SIZE 10240

class Myeasyserver{
private:
    SOCKET _sock;
    char _szRecv[BUFFER_SIZE]={};
    char _szMsgBuf[BUFFER_SIZE*10]={};
    int _lastPos=0;
    std::vector<SOCKET>g_clients;
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
    int RecvDate(SOCKET _csock);

    //相应网络消息
    virtual void OnNetMsg(SOCKET _csock,DataHeader* header);
    //发送数据
    int SendData(SOCKET _csock, DataHeader* header);
    void SendDataToAll(DataHeader* header);
};







#endif