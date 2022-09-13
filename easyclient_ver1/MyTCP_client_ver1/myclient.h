#ifndef _MYCLIENT_H_
#define _MYCLIENT_H_
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
#include"Messagehead.hpp"
#define BUFFER_SIZE 10240


class Myeasyclient{
private:
    SOCKET _sock=INVALID_SOCKET;
    char _szRecv[BUFFER_SIZE]={};
    char _szMsgBuf[BUFFER_SIZE*10]={};
    int _lastPos=0;
public:
    Myeasyclient(){}
    virtual ~Myeasyclient(){
        Close();
    }
    void initSocket();//≥ı ºªØSocket
    int Connect(const char * ip,unsigned short port);
    void Close();
    bool onRun();
    bool isRun();
    int RecvDate(SOCKET _cSock);
    void OnNetMsg(DataHeader* header);
    int SendData(DataHeader* header);
};







#endif
