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
        //fd_set fdWrite;
        //fd_set fdExp;

        FD_ZERO(&fdReads);
        // FD_ZERO(&fdWrite);
        // FD_ZERO(&fdExp);

        FD_SET(_sock,&fdReads);
        // FD_SET(_sock,&fdWrite);
        // FD_SET(_sock,&fdExp);

        //SOCKET maxsock=_sock;
        //
        // for(int n=(int)g_clients.size()-1;n>=0;n--){
        //     FD_SET(g_clients[n]->getsock(),&fdReads);
        //     if(maxsock<g_clients[n]->getsock()){
        //         maxsock=g_clients[n]->getsock();
        //     }
        // }
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
            return true;
        }
        // for(int n=(int)g_clients.size()-1;n>=0;n--){
        //     if(FD_ISSET(g_clients[n]->getsock(),&fdReads)){
        //         if(-1==RecvDate(g_clients[n])){
        //             auto iter=g_clients.begin()+n;
        //             if(iter!=g_clients.end()){
        //                 delete g_clients[n];
        //                 g_clients.erase(iter);
        //             }
        //         }
        //     }
        // }
        return true;
    }
    return false;
}

bool Myeasyserver::isRun(){
    return _sock!=INVALID_SOCKET;
}


int Myeasyserver::RecvDate(ClientSock* csock){
    // int nlen=(int)recv(csock->getsock(),_szRecv,sizeof(_szRecv),0);//取出头部信息，两个short长度
    // if(nlen<=0){
    //     printf("与服务器断开连接...\n");
    //     return -1;
    // }
    // memcpy(csock->Msgbuff()+csock->getlastPos(),_szRecv,nlen);
    // csock->setlastPos(csock->getlastPos()+nlen);
    // while(csock->getlastPos()>=sizeof(DataHeader)){
    //     DataHeader* header=(DataHeader*)csock->Msgbuff();
    //     if(csock->getlastPos()>=header->datalength){
    //         int nsize=csock->getlastPos()-header->datalength;//剩余未处理的数据长度
    //         OnNetMsg(csock->getsock(),header);
    //         memcpy(csock->Msgbuff(),csock->Msgbuff()+header->datalength,nsize);//将后续数据前移
    //         csock->setlastPos(nsize);
    //     }
    //     else{
    //         break;
    //     }
    // }
     return 0;
}

void Myeasyserver::getMsgOnClient(){
    auto t1=_curtime.getElapsedsecond();
    if(t1>1.0){   
        int recvcount=0;
        //std::mutex mux;
        //int count=0;
        for(auto x:_cellservers){
            //count+=x->getclientcount();
            //mux.lock();
            recvcount+=(x->_recvcount);            
            //mux.unlock();
            x->_recvcount=0;
        }       
        printf("time= %lf,socket<%d>,clients= %d ,recvcount=%d.\n",t1,_sock,g_clients.size(),recvcount); 
        _curtime.update();
    }
}
//处理消息内容
void Myeasyserver::OnNetMsg(SOCKET csock, DataHeader* header){
    
    //printf("now time is %lf\n",t1);
    switch (header->cmd)
    {
    case CMD_LOGIN : {
        Login* login = (Login*)header;
        
        //printf("收到客户端消息：CMD_LOGIN,数据长度：%d\n", login->datalength);
        //Login ret;
        //SendData(csock,&ret);
    }
    break;
    case CMD_LOGIN_RESULT: {
        LoginResult* login = (LoginResult*)header;
        //printf("收到客户端消息：CMD_LOGIN_RESULT,数据长度：%d\n", login->datalength);
        //LoginResult ret;
        //SendData(csock,&ret);
    }
    break;
    case CMD_LOGOUT: {
        Logout* login = (Logout*)header;
        //printf("收到客户端消息：CMD_LOGOUT,数据长度：%d\n", login->datalength);
        //Logout ret;
        //SendData(csock,&ret);
    }
    break;
    
    case CMD_LOGOUT_RESULT:{
        LogoutResult* logout=(LogoutResult*)header;
        //printf("收到客户端消息：CMD_LOGOUT_RESULT,数据长度：%d\n",logout->datalength);
        //LogoutResult ret;
        //SendData(csock,&ret);
    }
    break;
    case CMD_NEW_USER_JOIN:{
        NewUserJoin* userjoin=(NewUserJoin*)header;
        //printf("收到客户端消息：CMD_NEW_USER_JOIN,数据长度：%d\n",userjoin->datalength);
        //NewUserJoin ret;
        //SendData(csock,&ret);
    }
    break;
    case CMD_ERROR:{
        printf("收到错误消息,数据长度：%d\n",header->datalength);
        DataHeader header;
        SendData(csock,&header);
    }
    break;    
    default:
        printf("收到不明消息,数据长度：%d\n",header->datalength);
        DataHeader header;
        SendData(csock,&header);
        break;
    }
}
//发送给指定客户端的数据
int Myeasyserver::SendData(SOCKET csock,DataHeader* header){
    if(isRun()&&header){
        return send(csock,(const char*)header,header->datalength,0);
    }
    return SOCKET_ERROR;
}
//发送给所有客户端的数据
void Myeasyserver::SendDataToAll(DataHeader* header){
    for(auto clients:g_clients){
        SendData(clients->getsock(),header);
    }
}

void Myeasyserver::Clear_client(){

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