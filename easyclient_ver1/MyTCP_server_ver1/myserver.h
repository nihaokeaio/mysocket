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
    int RecvDate(SOCKET _csock);

    //��Ӧ������Ϣ
    virtual void OnNetMsg(SOCKET _csock,DataHeader* header);
    //��������
    int SendData(SOCKET _csock, DataHeader* header);
    void SendDataToAll(DataHeader* header);
};







#endif