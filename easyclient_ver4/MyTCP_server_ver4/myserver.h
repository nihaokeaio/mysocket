#ifndef _MYSERVER_H_
#define _MYSERVER_H_
#ifdef _WIN32
    #ifndef FD_SETSIZE
    #define FD_SETSIZE 1024
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
#define BUFFER_SIZE 10240
#endif

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

class Myeasyserver{
private:
    SOCKET _sock;
    char _szRecv[BUFFER_SIZE]={};
    std::vector<ClientSock*>g_clients;
    CELLTimestamp _curtime;
    int _recvcount;
public:
    Myeasyserver(){
        _sock=INVALID_SOCKET;
        _recvcount=0;
    }
    virtual ~Myeasyserver(){
        Close();
    }
    //��ʼ��socket
    void InitSocket();
    //�󶨶˿ں�
    SOCKET  Bind(const char* ip,unsigned short port);
    //�����˿ں�
    int Listen(int n);
    //��ʼ���ܿͻ�������
    int Accept();
    //�ر�socket
    void Close();
    //����������Ϣ
    bool OnRun();
    //�Ƿ��ڹ�����
    bool isRun();
    //�������ݣ�����ս������ְ�
    int RecvDate(ClientSock* csock);

    //��Ӧ������Ϣ
    virtual void OnNetMsg(SOCKET csock,DataHeader* header);
    //��������
    int SendData(SOCKET csock, DataHeader* header);
    void SendDataToAll(DataHeader* header);
    void Clear_client();
};







#endif