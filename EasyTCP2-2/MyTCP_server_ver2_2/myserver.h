#ifndef _MYSERVER_H_
#define _MYSERVER_H_
#ifdef _WIN32
    #ifndef FD_SETSIZE
    #define FD_SETSIZE 2506
    #endif
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <WinSock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <unstd.h>
    #include <arpa/inet.h>
    #include <string.h>

    #define SOCKET int
    #define INVALID_SOCKET (SOCKET)(~0)
    #define SOCKET_ERROR (-1)
#endif

#ifndef _ClASS_H
#define _CLASS_H
    #include <memory>
    #include <stdio.h>
    #include <vector>
    #include <set>
    #include <mutex>
    #include <thread>
    #include <atomic>
    #include <list>
    #include<map>
    
    #include"MemoryManage.hpp"
    #include "CELLTimestamp.h"
    #include "Messagehead.hpp"
    #include"CellTask.hpp"
    #include "Alloc.h"

    class CellTask;
    class ClientSock;
    class Cellserver;
    class Inetclient;
    class Cell2CTask;
    //class CelltaskServer;


    typedef std::shared_ptr<ClientSock> ClientSock_SMP;
    typedef std::shared_ptr<Cellserver> Cellserver_SMP;
    typedef std::shared_ptr<Inetclient> Inetclient_SMP;
    typedef std::shared_ptr<DataHeader> DataHeader_SMP;
    typedef std::shared_ptr<Cell2CTask> Cell2CTask_SMP;
    typedef std::shared_ptr<CellTask> CellTask_SMP;
    //typedef std::shared_ptr<CelltaskServer> CelltaskServer_SMP;
#endif



#ifndef RECV_BUFFER_SIZE
    #define RECV_BUFFER_SIZE 10240*5
    #define SEND_BUFFER_SIZE 10240*5
#endif
#define _SER_THREAD_COUNT 4



class Inetclient
{
public:
    virtual void OnNetMsg(Cellserver* pcellserver, ClientSock_SMP& client, DataHeader* header) = 0;
    virtual void ClientleaveOn(ClientSock_SMP& pclient)=0;
    virtual void Recvcount(ClientSock_SMP& client) = 0;
    virtual ~Inetclient() {}
};

//������Ϣ��������

//����ͻ��˵�Server
class Cellserver
{
private:
    //��ʵ�ͻ�����
    std::map<SOCKET,ClientSock_SMP> _clients;
    //����ͻ�����
    std::vector<ClientSock_SMP> _clientsBuff;
    SOCKET _sock;

    std::mutex _mutex;
    std::thread _pthread;
    Inetclient* _pNetEvent;
    CelltaskServer _taskserver;

public:
    Cellserver(SOCKET sock = INVALID_SOCKET)
    {
        _sock = sock;
        _pNetEvent = nullptr;
    }
    void setEventObj(Inetclient* pclient) {
        _pNetEvent = pclient;
    }
    ~Cellserver()
    {
        Close();
        printf("Cellserver  is exit !\n");
    }
    //�ر�socket
    void Close();
    //����������Ϣ
    bool OnRun();
    //�Ƿ��ڹ�����
    bool isRun();
    //�������ݣ�����ս������ְ�
    int RecvDate(ClientSock_SMP& csock);

    virtual void OnNetMsg(ClientSock_SMP& csock, DataHeader *header);
    // void Clear_client();
    void addClient(ClientSock_SMP& pclient);
    void start();
    size_t getclientcount();
    void addSendTask(ClientSock_SMP& csock, DataHeader_SMP& header);
};

//�Խӿͻ��˵�SOCKET
class ClientSock
{
private:
    char _szMsgBuf[RECV_BUFFER_SIZE];
    SOCKET _sockfd;
    int _lastPos;
    char _szSendBuf[SEND_BUFFER_SIZE];
    int _lastsendPos;
public:
    ClientSock(SOCKET sock= INVALID_SOCKET)
    {
        _sockfd = sock;
        _lastPos = 0;
        _lastsendPos = 0;
        memset(_szMsgBuf, 0, RECV_BUFFER_SIZE);
        memset(_szSendBuf, 0, SEND_BUFFER_SIZE);
    }
    ~ClientSock()
    {
        //printf("ClientSock _szMsgbuff is pop\n");
        Close();
    }
    SOCKET getsock()
    {
        return _sockfd;
    }
    char *Msgbuff()
    {
        return _szMsgBuf;
    }
    SOCKET SendData(DataHeader_SMP& header)
    {
        int ret = SOCKET_ERROR;
        //Ҫ���͵����ݳ���
        int nSendLen = header->datalength;
        //Ҫ���͵�����
        const char* pSendData = (const char*)header.get();

        while (true)
        {
            if (_lastsendPos + nSendLen >= SEND_BUFFER_SIZE)
            {
                //����ɿ��������ݳ���
                int nCopyLen = SEND_BUFFER_SIZE - _lastsendPos;
                //��������
                memcpy(_szSendBuf + _lastsendPos, pSendData, nCopyLen);
                //����ʣ������λ��
                pSendData += nCopyLen;
                //����ʣ�����ݳ���
                nSendLen -= nCopyLen;
                //��������
                ret = send(_sockfd, _szSendBuf, SEND_BUFFER_SIZE, 0);
                //����β��λ������
                _lastsendPos = 0;
                //���ʹ���
                if (SOCKET_ERROR == ret)
                {
                    return ret;
                }
            }
            else {
                //��Ҫ���͵����� ���������ͻ�����β��
                memcpy(_szSendBuf + _lastsendPos, pSendData, nSendLen);
                //��������β��λ��
                _lastsendPos += nSendLen;
                break;
            }
        }
        return ret;
    }
    void setlastPos(int pos = 0)
    {
        _lastPos = pos;
    }
    int getlastPos()
    {
        return _lastPos;
    }
    void Close()
    {
        if (_sockfd != INVALID_SOCKET)
        {
#ifdef _WIN32
            closesocket(_sockfd);
            //���windows socket����
#else
            Close();
#endif
            _sockfd = INVALID_SOCKET;
        }
    }
};

//����������
class Myeasyserver : public Inetclient
{
private:
    SOCKET _sock; 
    CELLTimestamp _curtime;
    // std::set<ClientSock*>g_clients;
    std::vector<Cellserver_SMP> _cellservers;
protected:
    std::atomic_int _msgcount;
    std::atomic_int _clientCount;
    std::atomic_int _recvcount;
public:
    Myeasyserver()
    {
        _clientCount = 0;
        _msgcount = 0;
        _recvcount = 0;
        _sock = INVALID_SOCKET;
    }
    virtual ~Myeasyserver()
    {
        Close();
    }
    //��ʼ��socket
    SOCKET InitSocket();
    //�󶨶˿ں�
    SOCKET Bind(const char *ip, unsigned short port);
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

    void start(int nCellServer);
    void AddclientToCellserver(ClientSock_SMP& pcllient);
    //��ӡ������Ϣ
    void getMsgOnClient();
    virtual void ClientleaveOn(ClientSock_SMP& pclient); 
    virtual void OnNetJoin(ClientSock_SMP& plient);
    //virtual void Msgcount(Cellserver* pcellserver, ClientSock_SMP& client, DataHeader* header);
    virtual void Recvcount(ClientSock_SMP& client);
    virtual void OnNetMsg(Cellserver* pcellserver, ClientSock_SMP& client, DataHeader* header);
};


class Cell2CTask : public CellTask
{
private:
    ClientSock_SMP _pClient;
    DataHeader_SMP _pHeader;

public:
    Cell2CTask(ClientSock_SMP pClient, DataHeader_SMP& pHeader)
    {
        _pClient = pClient;
        _pHeader = pHeader;
    }
    virtual void doTask()
    {
        _pClient->SendData(_pHeader);
    }
};


#endif