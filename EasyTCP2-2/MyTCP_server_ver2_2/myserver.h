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

//网络消息发送任务

//处理客户端的Server
class Cellserver
{
private:
    //真实客户队列
    std::map<SOCKET,ClientSock_SMP> _clients;
    //缓冲客户队列
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
    //关闭socket
    void Close();
    //处理网络消息
    bool OnRun();
    //是否在工作中
    bool isRun();
    //接收数据，处理战报，拆分包
    int RecvDate(ClientSock_SMP& csock);

    virtual void OnNetMsg(ClientSock_SMP& csock, DataHeader *header);
    // void Clear_client();
    void addClient(ClientSock_SMP& pclient);
    void start();
    size_t getclientcount();
    void addSendTask(ClientSock_SMP& csock, DataHeader_SMP& header);
};

//对接客户端的SOCKET
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
        //要发送的数据长度
        int nSendLen = header->datalength;
        //要发送的数据
        const char* pSendData = (const char*)header.get();

        while (true)
        {
            if (_lastsendPos + nSendLen >= SEND_BUFFER_SIZE)
            {
                //计算可拷贝的数据长度
                int nCopyLen = SEND_BUFFER_SIZE - _lastsendPos;
                //拷贝数据
                memcpy(_szSendBuf + _lastsendPos, pSendData, nCopyLen);
                //计算剩余数据位置
                pSendData += nCopyLen;
                //计算剩余数据长度
                nSendLen -= nCopyLen;
                //发送数据
                ret = send(_sockfd, _szSendBuf, SEND_BUFFER_SIZE, 0);
                //数据尾部位置清零
                _lastsendPos = 0;
                //发送错误
                if (SOCKET_ERROR == ret)
                {
                    return ret;
                }
            }
            else {
                //将要发送的数据 拷贝到发送缓冲区尾部
                memcpy(_szSendBuf + _lastsendPos, pSendData, nSendLen);
                //计算数据尾部位置
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
            //清楚windows socket环境
#else
            Close();
#endif
            _sockfd = INVALID_SOCKET;
        }
    }
};

//监听服务器
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
    //初始化socket
    SOCKET InitSocket();
    //绑定端口号
    SOCKET Bind(const char *ip, unsigned short port);
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

    void start(int nCellServer);
    void AddclientToCellserver(ClientSock_SMP& pcllient);
    //打印接受信息
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