#include "myserver.h"



void Cellserver::Close()
{
    if (_sock != INVALID_SOCKET)
    {
#ifdef _WIN32
        for (auto iter : _clients)
        {
            closesocket(iter.first);
        }
        closesocket(_sock);
#else
        for (auto iter : _clients)
        {
            close(iter.first);
        }
        //关闭套节字closesocket
        close(_sock);
#endif
        _sock = INVALID_SOCKET;
        _clients.clear();
    }
}

bool Cellserver::OnRun()
{
    fd_set fdReads, allReads;
    SOCKET maxsock = _sock;
    FD_ZERO(&fdReads);
    FD_ZERO(&allReads);
    bool Need_scan = false;
    while (isRun())
    {
        if (!_clientsBuff.empty() || Need_scan)
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                for (auto x : _clientsBuff)
                {
                    _clients[x->getsock()] = x;
                }
            }
            _clientsBuff.clear();
            FD_ZERO(&fdReads);
            FD_ZERO(&allReads);
            for (auto iter : _clients)
            {
                FD_SET(iter.first, &fdReads);
                if (maxsock < iter.first)
                {
                    maxsock = iter.first;
                }
            }
            Need_scan = false;
            allReads = fdReads;
        }
        else
        {
            fdReads = allReads;
        }
        if (_clients.empty())
        {
            std::chrono::milliseconds t(10);
            std::this_thread::sleep_for(t);
            continue;
        }

        timeval t = { 1, 0 };
        int ret = select(maxsock + 1, &fdReads, nullptr, nullptr, &t);
        if (ret < 0)
        {
            printf("socket=%d,select 任务结束...\n", getclientcount());
            Close();
            return false;
        }
#ifdef _WIN32
        std::vector<SOCKET> temp;
        for (auto iter : _clients)
        {
            if (FD_ISSET(iter.first, &fdReads))
            {
                if (-1 == RecvDate(iter.second))
                {
                    if (_pNetEvent)
                    {
                        _pNetEvent->ClientleaveOn(iter.second);
                    }
                    Need_scan = true;
                    temp.push_back(iter.first);
                }
            }
        }
        for (auto pClient : temp)
        {
            _clients.erase(pClient);
        }
        /*
        for (int n = 0; n < fdReads.fd_count; n++)
        {
            auto iter = _clients.find(fdReads.fd_array[n]);
            if (iter != _clients.end())
            {
                if (-1 == RecvDate(iter->second))
                {
                    //if (_pNetEvent)
                        //_pNetEvent->ClientleaveOn(iter->second);
                    Need_scan = true;
                    _clients.erase(iter->first);
                }
            }
            else {
                printf("error. if (iter != _clients.end())\n");
            }

        }
        */
        
#else 

        std::vector<SOCKET> temp;
        for (auto iter : _clients)
        {
            if (FD_ISSET(iter.first, &fdReads))
            {
                if (-1 == RecvDate(iter.second))
                {
                    if (_pNetEvent)
                    {
                        _pNetEvent->ClientleaveOn(iter.second);
                    }
                    Need_scan = true;
                    temp.push_back(iter.first);
                }
                else {
                    printf("error. if (iter != _clients.end())\n");
                }
            }
        }
        for (auto pClient : temp)
        {
            _clients.erase(pClient);
        }
#endif        
    }

    return false;
}

bool Cellserver::isRun()
{
    return _sock != INVALID_SOCKET;
}

int Cellserver::RecvDate(ClientSock_SMP& csock)
{
    char* szRecv = csock->Msgbuff() + csock->getlastPos();
    int nlen = (int)recv(csock->getsock(), szRecv,(RECV_BUFFER_SIZE)-csock->getlastPos(),0); //取出头部信息，两个short长度  
    if (nlen <= 0)
    {
        //printf("socket <%d> 与服务器断开连接...\n", csock->getsock());
        //_pNetEvent->ClientleaveOn(csock);
        return -1;
    }
    _pNetEvent->Recvcount(csock);
    //csock->setlastPos(csock->getlastPos() + nlen);
    //memcpy(csock->Msgbuff() + csock->getlastPos(), _szRecv, nlen);
    csock->setlastPos(csock->getlastPos() + nlen);
    while (csock->getlastPos() >= sizeof(DataHeader))
    {
        DataHeader *header = (DataHeader *)csock->Msgbuff();
        if (csock->getlastPos() >= header->datalength)
        {
            int nsize = csock->getlastPos() - header->datalength; //剩余未处理的数据长度
            OnNetMsg(csock, header);
            memcpy(csock->Msgbuff(), csock->Msgbuff() + header->datalength, nsize); //将后续数据前移
            csock->setlastPos(nsize);
        }
        else
        {
            break;
        }
    }
    return 0;
}

//处理消息内容
void Cellserver::OnNetMsg(ClientSock_SMP& csock, DataHeader *header)
{
    _pNetEvent->OnNetMsg(this,csock,header);
    /*
    switch (header->cmd)
    {
    case CMD_LOGIN:
    {
        Login *login = (Login *)header;

        // printf("收到客户端消息：CMD_LOGIN,数据长度：%d\n", login->datalength);
        Login *ret = new Login();
        auto task = std::make_shared<Cell2CTask>(csock, ret);
        _taskserver.addtask(task);
    }
    break;
    case CMD_LOGIN_RESULT:
    {
        LoginResult *login = (LoginResult *)header;
        // printf("收到客户端消息：CMD_LOGIN_RESULT,数据长度：%d\n", login->datalength);
        // LoginResult ret;
        // SendData(csock,&ret);
    }
    break;
    case CMD_LOGOUT:
    {
        Logout *login = (Logout *)header;
        // printf("收到客户端消息：CMD_LOGOUT,数据长度：%d\n", login->datalength);
        // Logout ret;
        // SendData(csock,&ret);
    }
    break;

    case CMD_LOGOUT_RESULT:
    {
        LogoutResult *logout = (LogoutResult *)header;
        // printf("收到客户端消息：CMD_LOGOUT_RESULT,数据长度：%d\n",logout->datalength);
        // LogoutResult ret;
        // SendData(csock,&ret);
    }
    break;
    case CMD_NEW_USER_JOIN:
    {
        NewUserJoin *userjoin = (NewUserJoin *)header;
        // printf("收到客户端消息：CMD_NEW_USER_JOIN,数据长度：%d\n",userjoin->datalength);
        // NewUserJoin ret;
        // SendData(csock,&ret);
    }
    break;
    case CMD_ERROR:
    {
        printf("收到错误消息,数据长度：%d\n", header->datalength);
        DataHeader *header = new DataHeader();
        SendData(csock, header);
        // SendData(csock,&header);
    }
    break;
    default:
        printf("收到不明消息,数据长度：%d\n", header->datalength);
        DataHeader *header = new DataHeader();
        SendData(csock, header);
        break;
    }
    */
}


void Cellserver::start()
{
    _pthread = std::thread(&Cellserver::OnRun, this);
    _pthread.detach();//我吐了。。。
    _taskserver.Start();
}

void Cellserver::addClient(ClientSock_SMP& pclient)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _clientsBuff.push_back(pclient);
}

typedef std::shared_ptr<Cell2CTask> Cell2CTask_SMP;
void Cellserver::addSendTask(ClientSock_SMP& pclient, DataHeader_SMP& header)
{
    CellTask_SMP task = std::make_shared<Cell2CTask >(pclient, header);
    _taskserver.addtask(task);
} 

size_t Cellserver::getclientcount()
{
    return _clients.size() + _clientsBuff.size();
}
