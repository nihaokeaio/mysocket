#include"myserver.h"
//int Cellserver::_recvcount=0;


void Cellserver::Close(){
    Clear_client();
    if(_sock!=INVALID_SOCKET){
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

bool Cellserver::OnRun(){
    while(isRun()){
        if(!_clientsBuff.empty()){
            std::lock_guard<std::mutex>lock(_mutex);
            for(auto x:_clientsBuff){
                _clients.push_back(x);
            }
            _clientsBuff.clear();
        }
        if(_clients.empty()){
            //std::chrono::milliseconds t(1);
            //std::this_thread::sleep_for(t);
            continue;
        }
        fd_set fdReads;
        fd_set fdWrite;
        fd_set fdExp;

        FD_ZERO(&fdReads);
        FD_ZERO(&fdWrite);
        FD_ZERO(&fdExp);
        FD_SET(_sock,&fdReads);
        FD_SET(_sock,&fdWrite);
        FD_SET(_sock,&fdExp);

        SOCKET maxsock=_sock;
        
        for(int n=(int)_clients.size()-1;n>=0;n--){
            FD_SET(_clients[n]->getsock(),&fdReads);
            if(maxsock<_clients[n]->getsock()){
                maxsock=_clients[n]->getsock();
            }
        }
        timeval t={1,0};
        int ret =select(maxsock+1,&fdReads,&fdWrite,&fdExp,&t);
        if(ret<0){
            printf("socket=%d,select 任务结束...\n",getclientcount());
            Close();
            return false;
        }
        for(int n=(int)_clients.size()-1;n>=0;n--){
            if(FD_ISSET(_clients[n]->getsock(),&fdReads)){
                if(-1==RecvDate(_clients[n])){
                    auto iter=_clients.begin()+n;
                    if(iter!=_clients.end()){
                        delete _clients[n];
                        _clients.erase(iter);
                    }
                }
            }
        }
    }
    return false;
}

bool Cellserver::isRun(){
    return _sock!=INVALID_SOCKET;
}


int Cellserver::RecvDate(ClientSock* csock){
    int nlen=(int)recv(csock->getsock(),_szRecv,sizeof(_szRecv),0);//取出头部信息，两个short长度
    if(nlen<=0){
        printf("socket <%d> 与服务器断开连接...\n",csock->getsock());
        return -1;
    }
    memcpy(csock->Msgbuff()+csock->getlastPos(),_szRecv,nlen);
    csock->setlastPos(csock->getlastPos()+nlen);
    while(csock->getlastPos()>=sizeof(DataHeader)){
        DataHeader* header=(DataHeader*)csock->Msgbuff();
        if(csock->getlastPos()>=header->datalength){
            int nsize=csock->getlastPos()-header->datalength;//剩余未处理的数据长度
            OnNetMsg(csock->getsock(),header);
            memcpy(csock->Msgbuff(),csock->Msgbuff()+header->datalength,nsize);//将后续数据前移
            csock->setlastPos(nsize);
        }
        else{
            break;
        }
    }
    return 0;
}

//处理消息内容
void Cellserver::OnNetMsg(SOCKET csock, DataHeader* header){
    
    //_mutex.lock();
    _recvcount++;
    //printf("_recvcount=%d\n",_recvcount);
    //_mutex.unlock();
    // auto t1=_curtime.getElapsedsecond();
    // if(t1>1.0){
    //     _curtime.update();
    //     printf("time= %lf,socket<%d>,clients= %d ,recvcount=%d.\n",t1,csock,_clients.size(), _recvcount);
    //     _recvcount=0;
    // }
    
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
int Cellserver::SendData(SOCKET csock,DataHeader* header){
    if(isRun()&&header){
        return send(csock,(const char*)header,header->datalength,0);
    }
    return SOCKET_ERROR;
}
//发送给所有客户端的数据
void Cellserver::SendDataToAll(DataHeader* header){
    for(int n=(int)_clients.size()-1;n>=0;n--){
        SendData(_clients[n]->getsock(),header);
    }
}

void Cellserver::Clear_client(){
    for(int n=(int)_clients.size()-1;n>=0;n--){
        delete _clients[n];
        _clients[n]=nullptr;
    }
}
  
void Cellserver::start(){
    _pthread=new std::thread(&Cellserver::OnRun,this); 
}

void Cellserver::addClient(ClientSock* pclient){
        std::lock_guard<std::mutex>lock(_mutex);
        _clientsBuff.push_back(pclient);
}


size_t Cellserver::getclientcount(){
    return _clients.size()+_clientsBuff.size();
}


