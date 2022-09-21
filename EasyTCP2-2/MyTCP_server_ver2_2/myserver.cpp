#include "myserver.h"

SOCKET Myeasyserver::InitSocket()
{
    //初始化，启动win，环境
#ifdef _WIN32
    WORD ver = MAKEWORD(2, 2);
    WSADATA dat;
    WSAStartup(ver, &dat);
#endif
    if (_sock != INVALID_SOCKET)
    {
        printf("关闭旧连接<socket=%d>...\n", _sock);
        Close();
    }
    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == _sock)
    {
        printf("错误,建立Socket失败...\n");
    }
    else
    {
        printf("建立<Socket=%d>成功...\n", _sock);
    }
    return _sock;
}

SOCKET Myeasyserver::Bind(const char *ip, unsigned short port)
{
    if (_sock == INVALID_SOCKET)
    {
        InitSocket();
    }
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(port);

#ifdef _WIN32
    if (ip)
    {
        _sin.sin_addr.S_un.S_addr = inet_addr(ip);
    }
    else
    {
        _sin.sin_addr.S_un.S_addr = INADDR_ANY;
    }
#else
    if (ip)
    {
        _sin.sin_addr.s_addr = inet_addr(ip);
    }
    else
    {
        _sin.sin_addr.s_addr = INADDR_ANY;
    }
#endif
    int ret = bind(_sock, (sockaddr *)&_sin, sizeof(sockaddr_in));
    // printf("res=%d\n",ret);
    if (SOCKET_ERROR == ret)
    {
        printf("错误,绑定端口<%d>失败...\n", port);
    }
    else
    {
        printf("绑定端口<%d>成功...\n", port);
    }
    return ret;
}

int Myeasyserver::Listen(int n)
{
    int ret = listen(_sock, n);
    if (SOCKET_ERROR == ret)
    {
        printf("Socket=<%d>错误，监听网络失败...\n", _sock);
    }
    else
    {
        printf("Socket=<%d>监听网络成功...\n", _sock);
    }
    return ret;
}

int Myeasyserver::Accept()
{
    sockaddr_in clientAddr = {};
    int nAddrLen = sizeof(sockaddr_in);
    SOCKET csock = INVALID_SOCKET;
#ifdef _WIN32
    csock = accept(_sock, (sockaddr *)&clientAddr, &nAddrLen);
#else
    _csock = accept(_sock, (sockaddr *)&clientAddr, (socklen_t *)&nAddrLen);
#endif
    if (INVALID_SOCKET == csock)
    {
        printf("Socket=<%d>错误，接受无效的客户端连接...\n", csock);
    }
    else
    {
        // NewUserJoin userJoin;
        // SendDataToAll(&userJoin);
        ClientSock_SMP client = std::make_shared<ClientSock>(csock);
        AddclientToCellserver(client);
        // printf("socket=<%d> 新客户加入,IP = %s\n",(int)csock,inet_ntoa(clientAddr.sin_addr));
    }
    return csock;
}
typedef std::shared_ptr<ClientSock> ClientSock_SMP;
void Myeasyserver::AddclientToCellserver(ClientSock_SMP& pclient)
{
    OnNetJoin(pclient);
    auto pMinserver = _cellservers[0];
    for (auto x : _cellservers)
    {
        if (x->getclientcount() < pMinserver->getclientcount())
            pMinserver = x;
    }
    pMinserver->addClient(pclient);
}
void Myeasyserver::Close()
{
    if (_sock != INVALID_SOCKET)
    {      
#ifdef _WIN32
        closesocket(_sock);
        //清楚windows socket环境
        WSACleanup();
#else
        close(_sock);
#endif
        _sock = INVALID_SOCKET;
    }
}

bool Myeasyserver::OnRun()
{
    if (isRun())
    {
        getMsgOnClient();
        fd_set fdReads;
        FD_ZERO(&fdReads);

        FD_SET(_sock, &fdReads);

        
        timeval t = {0, 0};
        int ret = select(_sock + 1, &fdReads, nullptr, nullptr, &t);
        if (ret < 0)
        {
            printf("<socket=%d>select 任务结束...\n", _sock);
            Close();
            return false;
        }
        if (FD_ISSET(_sock, &fdReads))
        {
            FD_CLR(_sock, &fdReads);
            Accept();
        }
        return true;
    }
    return false;
}

bool Myeasyserver::isRun()
{
    return _sock != INVALID_SOCKET;
}

void Myeasyserver::getMsgOnClient()
{
    auto t1 = _curtime.getElapsedsecond();
    if (t1 > 1.0)
    {
        // std::mutex mux;
        int recvcount = _recvcount;
        int msgcount = _msgcount;
        int clientCount = _clientCount;
        printf("time= %lf,socket<%d>,clients=%d,msgcount =%d,recvcount=%d.\n", t1, _sock, clientCount, msgcount, recvcount);
        _recvcount = 0;
        _msgcount = 0;
        _curtime.update();
    }
}

void Myeasyserver::start(int nCellServer)
{
    using namespace std;
    for (int i = 0; i < nCellServer; i++)
    {
        //_cellservers[i] = new Cellserver(_sock, this);
        auto ser = std::make_shared<Cellserver>(_sock);
        _cellservers.push_back(ser);
        ser->setEventObj(this);       
        ser->start();
    }
}
void Myeasyserver::ClientleaveOn(ClientSock_SMP& pclient)
{
    _clientCount--;
}

void Myeasyserver::OnNetMsg(Cellserver* pcellserver,ClientSock_SMP& client,DataHeader* header)
{
    _msgcount++;
}

void Myeasyserver::Recvcount(ClientSock_SMP& client)
{
    _recvcount++;
}

void Myeasyserver::OnNetJoin(ClientSock_SMP& plient)
{
    _clientCount++;
}