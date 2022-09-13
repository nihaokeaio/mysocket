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
#include"CELLTimestamp.h"
#include<atomic>
#include<stdio.h>
#include<mutex>
#include"Messagehead.hpp"
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif


class Myeasyclient{
private:
    SOCKET _sock;
    char *_szRecv=new char[BUFFER_SIZE];
    char *_szMsgBuf=new char[BUFFER_SIZE*10];
    int _lastPos=0;
    static std::atomic_int _msgcount;
    std::mutex _mutex;
public:
    
    Myeasyclient(){
        _msgcount=0;
        _sock=INVALID_SOCKET;
        memset(_szRecv,0,sizeof(_szRecv));
        memset(_szMsgBuf,0,sizeof(_szMsgBuf));
    }
    virtual ~Myeasyclient(){
        delete _szMsgBuf;
        delete _szRecv;
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
    int Getsock(){
        return _sock;
    }
    static int Getcount();
    static void Addcount();
    static void Resetcount();
};







#endif
