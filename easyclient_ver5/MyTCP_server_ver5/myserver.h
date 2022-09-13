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


//����ͻ��˵�Server
class Cellserver{
private:
    //��ʵ�ͻ�����
    std::vector<ClientSock*>_clients;
    //����ͻ�����
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
    void addClient(ClientSock* pclient);
    void start();
    size_t getclientcount();
};

//�Խӿͻ��˵�SOCKET
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


//����������
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

    void start();
    void AddclientToCellserver(ClientSock* pcllient);
    //��ӡ������Ϣ
    void getMsgOnClient();
};







#endif