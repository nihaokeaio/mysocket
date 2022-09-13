#include"myserver.h"


void Cellserver::Close() {
    if (_sock != INVALID_SOCKET) {
        for (int n = (int)_clients.size() - 1; n >= 0; n--) {
            if (_clients[n] != nullptr) {
                delete _clients[n];
                _clients[n] = nullptr;
            }
        }
        _clients.erase(_clients.begin(), _clients.end());
    }
}

bool Cellserver::OnRun(){
    fd_set fdReads, allReads;
    SOCKET maxsock = _sock;
    FD_ZERO(&fdReads);
    FD_ZERO(&allReads);
    bool Need_scan = false;
    while(isRun()){
        if (!_clientsBuff.empty()||Need_scan) {
            {
                std::lock_guard<std::mutex>lock(_mutex);
                for (auto x : _clientsBuff) {
                    _clients.push_back(x);
                }
            }
            _clientsBuff.clear();
            FD_ZERO(&fdReads);
            FD_ZERO(&allReads);
            for (int n = (int)_clients.size() - 1; n >= 0; n--) {
                FD_SET(_clients[n]->getsock(), &fdReads);
                if (maxsock < _clients[n]->getsock()) {
                    maxsock = _clients[n]->getsock();
                }
            }
            Need_scan = false;
            allReads = fdReads;
        }
        else
        {
            fdReads = allReads;
        }
        if(_clients.empty()){
            std::chrono::milliseconds t(10);
            std::this_thread::sleep_for(t);
            continue;
        }
     
        timeval t={1,0};
        int ret =select(maxsock+1,&fdReads,nullptr,nullptr,&t);
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
                        _clients.erase(iter);
                        Need_scan = true;
                    }
                }
            }
        }
    }
    return false;
}

bool Cellserver::isRun(){
    return _sock!=INVALID_SOCKET&& cmdOnRun;
}


int Cellserver::RecvDate(ClientSock* csock){
    int nlen=(int)recv(csock->getsock(),_szRecv,sizeof(_szRecv),0);//取出头部信息，两个short长度
    if(nlen<=0){
        printf("socket <%d> 与服务器断开连接...\n",csock->getsock()); 
        OnMsgToleave(csock);
        //csock->Close();
        if (csock != nullptr) {
            delete csock;
            csock = nullptr;
        }
        return -1;
    }
    OnRecvCount();
    memcpy(csock->Msgbuff()+csock->getlastPos(),_szRecv,nlen);
    csock->setlastPos(csock->getlastPos()+nlen);
    while(csock->getlastPos()>=sizeof(DataHeader)){
        DataHeader* header=(DataHeader*)csock->Msgbuff();
        if(csock->getlastPos()>=header->datalength){
            int nsize=csock->getlastPos()-header->datalength;//剩余未处理的数据长度
            OnNetMsg(csock,header);
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
void Cellserver::OnNetMsg(ClientSock* csock, DataHeader* header){
    OnMsgCount();
    switch (header->cmd)
    {
    case CMD_LOGIN : {
        Login* login = (Login*)header;
        
        //printf("收到客户端消息：CMD_LOGIN,数据长度：%d\n", login->datalength);
        Login* ret= new Login();
        CellSendMsg2clientTask* task=new CellSendMsg2clientTask(csock,ret);
        _taskserver.addtask(task);
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
        DataHeader* header = new DataHeader();
        SendData(csock,header);
        //SendData(csock,&header);
    }
    break;    
    default:
        printf("收到不明消息,数据长度：%d\n",header->datalength);
        DataHeader* header = new DataHeader();
        SendData(csock,header);
        break;
    }
}
//发送给指定客户端的数据
int Cellserver::SendData(ClientSock* csock,DataHeader* header){
    if(isRun()&&header){
        return send(csock->getsock(),(const char*)header,header->datalength,0);
    }
    return SOCKET_ERROR;
}
//发送给所有客户端的数据
void Cellserver::SendDataToAll(DataHeader* header){
    for(int n=(int)_clients.size()-1;n>=0;n--){
        SendData(_clients[n],header);
    }
}

  
void Cellserver::start(){
    _pthread=std::thread(&Cellserver::OnRun,this);
    _taskserver.Start();
}

void Cellserver::addClient(ClientSock* pclient){
        std::lock_guard<std::mutex>lock(_mutex);
        _clientsBuff.push_back(pclient);
}

void Cellserver::addSendTask(CellTask* task){
    _taskserver.addtask(task);
}


size_t Cellserver::getclientcount(){
    return _clients.size()+_clientsBuff.size();
}

void Cellserver::OnMsgToleave(ClientSock* plient){
    _pclientleave->ClientleaveOn(plient);
}

void Cellserver::OnMsgCount(){
    _pclientleave->Msgcount();
}

void Cellserver::OnRecvCount(){
    _pclientleave->Recvcount();
}