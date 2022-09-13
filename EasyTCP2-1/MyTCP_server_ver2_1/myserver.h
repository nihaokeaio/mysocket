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

//������Ϣ��������



//����ͻ��˵�Server
class Cellserver{
private:
    //��ʵ�ͻ�����
    std::vector<ClientSock*>_clients;
    //����ͻ�����
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
    //�ر�socket
    void Close();
    //����������Ϣ
    bool OnRun();
    //�Ƿ��ڹ�����
    bool isRun();
    //�������ݣ�����ս������ְ�
    int RecvDate(ClientSock* csock);

    virtual void OnNetMsg(ClientSock*,DataHeader* header);
    //��������
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

//�Խӿͻ��˵�SOCKET
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
        //���windows socket����
    #else
        Close();
    #endif
        _sock=INVALID_SOCKET;
        }
    }
};


//����������
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

    void start();
    void AddclientToCellserver(ClientSock* pcllient);
    //��ӡ������Ϣ
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