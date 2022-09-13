#include"myserver.h"

void Myeasyserver::InitSocket(){
        //初始化，启动win，环境 
#ifdef _WIN32
    WORD ver = MAKEWORD(2,2);
    WSADATA dat;
    WSAStartup(ver,&dat);
#endif
    if(_sock!=INVALID_SOCKET){
        printf("关闭旧连接<socket=%d>...\n",_sock);
        Close();
    }
    _sock=socket(AF_INET,SOCK_STREAM,0);
    if(INVALID_SOCKET==_sock){
        printf("错误,建立Socket失败...\n");
    }
    else{
        printf("建立<Socket=%d>成功...\n",_sock);
    }
}

SOCKET  Myeasyserver::Bind(const char* ip,unsigned short port){
    if(_sock==INVALID_SOCKET){
        InitSocket();
    }
    sockaddr_in _sin={};
    _sin.sin_family=AF_INET;
    _sin.sin_port=htons(port);
    
#ifdef _WIN32
    if(ip){
        _sin.sin_addr.S_un.S_addr=inet_addr(ip);
    }
    else{
        _sin.sin_addr.S_un.S_addr=INADDR_ANY;
    }
#else
    if(ip){
        _sin.sin_addr.s_addr=inet_addr(ip);
    }
    else{
         _sin.sin_addr.s_addr=INADDR_ANY;
    }
#endif
    int ret=bind(_sock,(sockaddr*)&_sin,sizeof(sockaddr_in));
    //printf("res=%d\n",ret);
    if(SOCKET_ERROR==ret){
        printf("错误,绑定端口<%d>失败...\n",port);
    }
    else{
        printf("绑定端口<%d>成功...\n",port);
    }
    return ret;
}

int Myeasyserver::Listen(int n){
    int ret=listen(_sock,n);
    if(SOCKET_ERROR == ret){
        printf("Socket=<%d>错误，监听网络失败...\n",_sock);
    }
    else{
        printf("Socket=<%d>监听网络成功...\n",_sock);
    }
    return ret;
}


int Myeasyserver::Accept(){
    sockaddr_in clientAddr={};
    int nAddrLen=sizeof(sockaddr_in);
    SOCKET csock =INVALID_SOCKET;
#ifdef _WIN32
    csock =accept(_sock,(sockaddr*)&clientAddr,&nAddrLen);
#else
    _csock=accept(_sock,(sockaddr*)&clientAddr,(socklen_t*)&nAddrLen);
#endif
    if(INVALID_SOCKET== csock){
        printf("Socket=<%d>错误，接受无效的客户端连接...\n", csock);
    }
    else{
        //NewUserJoin userJoin;
        //SendDataToAll(&userJoin);
        AddclientToCellserver(new ClientSock(csock));
        //printf("socket=<%d> 新客户加入,IP = %s\n",(int)csock,inet_ntoa(clientAddr.sin_addr));
    }
    return csock;
}
void Myeasyserver::AddclientToCellserver(ClientSock* pclient){
    g_clients.insert(pclient);
    auto pMinserver=_cellservers[0];
        for(auto x: _cellservers){
            if(x->getclientcount()<pMinserver->getclientcount())pMinserver=x;
        }
    pMinserver->addClient(pclient);
}
void Myeasyserver::Close(){
    if(_sock!=INVALID_SOCKET){
        int sizenum= _cellservers.size();
        for (int n =0; n< sizenum; n++) {
            delete _cellservers[n];
            _cellservers[n] = nullptr;
        }
    _cellservers.erase(_cellservers.begin(),_cellservers.end());
#ifdef _WIN32
    closesocket(_sock);
    //清楚windows socket环境
    WSACleanup();
#else
    close(_sock);
#endif
    _sock=INVALID_SOCKET;
    }
}

bool Myeasyserver::OnRun(){
    if(isRun()){
        fd_set fdReads;
        FD_ZERO(&fdReads);

        FD_SET(_sock,&fdReads);

        getMsgOnClient();
        timeval t={0,0};
        int ret =select(_sock+1,&fdReads,nullptr,nullptr,&t);
        if(ret<0){
            printf("<socket=%d>select 任务结束...\n",_sock);
            Close();
            return false;
        }
        if(FD_ISSET(_sock,&fdReads)){
            FD_CLR(_sock,&fdReads);
            Accept();
        }
        return true;
    }
    return false;
}

bool Myeasyserver::isRun(){
    return _sock!=INVALID_SOCKET;
}


int Myeasyserver::RecvDate(ClientSock* csock){  
     return 0;
}

void Myeasyserver::getMsgOnClient(){
    auto t1=_curtime.getElapsedsecond();
    if(t1>1.0){   
        //std::mutex mux;
        int recvcount=_recvcount; 
        int msgcount=_msgcount; 
        printf("time= %lf,socket<%d>,clients=%d,msgcount =%d,recvcount=%d.\n",t1,_sock,g_clients.size(),msgcount,recvcount);
        _recvcount=0; 
        _msgcount=0;
        _curtime.update();
    }
}


void Myeasyserver::start(){
    for(int i=0;i<_SER_THREAD_COUNT;i++){
        auto ser=new Cellserver(_sock,this);
        _cellservers.push_back(ser);
        ser->start();
    }
}
void Myeasyserver::ClientleaveOn(ClientSock* pclient){
    if(g_clients.find(pclient)!=g_clients.end()){
        g_clients.erase(pclient);
    }
}

void Myeasyserver::Msgcount(){
    _msgcount++;
}

void Myeasyserver::Recvcount(){
    _recvcount++;
}