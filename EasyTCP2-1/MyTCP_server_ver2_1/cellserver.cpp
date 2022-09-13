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
            printf("socket=%d,select �������...\n",getclientcount());
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
    int nlen=(int)recv(csock->getsock(),_szRecv,sizeof(_szRecv),0);//ȡ��ͷ����Ϣ������short����
    if(nlen<=0){
        printf("socket <%d> ��������Ͽ�����...\n",csock->getsock()); 
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
            int nsize=csock->getlastPos()-header->datalength;//ʣ��δ��������ݳ���
            OnNetMsg(csock,header);
            memcpy(csock->Msgbuff(),csock->Msgbuff()+header->datalength,nsize);//����������ǰ��
            csock->setlastPos(nsize);
        }
        else{
            break;
        }
    }
    return 0;
}

//������Ϣ����
void Cellserver::OnNetMsg(ClientSock* csock, DataHeader* header){
    OnMsgCount();
    switch (header->cmd)
    {
    case CMD_LOGIN : {
        Login* login = (Login*)header;
        
        //printf("�յ��ͻ�����Ϣ��CMD_LOGIN,���ݳ��ȣ�%d\n", login->datalength);
        Login* ret= new Login();
        CellSendMsg2clientTask* task=new CellSendMsg2clientTask(csock,ret);
        _taskserver.addtask(task);
    }
    break;
    case CMD_LOGIN_RESULT: {
        LoginResult* login = (LoginResult*)header;
        //printf("�յ��ͻ�����Ϣ��CMD_LOGIN_RESULT,���ݳ��ȣ�%d\n", login->datalength);
        //LoginResult ret;
        //SendData(csock,&ret);
    }
    break;
    case CMD_LOGOUT: {
        Logout* login = (Logout*)header;
        //printf("�յ��ͻ�����Ϣ��CMD_LOGOUT,���ݳ��ȣ�%d\n", login->datalength);
        //Logout ret;
        //SendData(csock,&ret);
    }
    break;
    
    case CMD_LOGOUT_RESULT:{
        LogoutResult* logout=(LogoutResult*)header;
        //printf("�յ��ͻ�����Ϣ��CMD_LOGOUT_RESULT,���ݳ��ȣ�%d\n",logout->datalength);
        //LogoutResult ret;
        //SendData(csock,&ret);
    }
    break;
    case CMD_NEW_USER_JOIN:{
        NewUserJoin* userjoin=(NewUserJoin*)header;
        //printf("�յ��ͻ�����Ϣ��CMD_NEW_USER_JOIN,���ݳ��ȣ�%d\n",userjoin->datalength);
        //NewUserJoin ret;
        //SendData(csock,&ret);
    }
    break;
    case CMD_ERROR:{
        printf("�յ�������Ϣ,���ݳ��ȣ�%d\n",header->datalength);
        DataHeader* header = new DataHeader();
        SendData(csock,header);
        //SendData(csock,&header);
    }
    break;    
    default:
        printf("�յ�������Ϣ,���ݳ��ȣ�%d\n",header->datalength);
        DataHeader* header = new DataHeader();
        SendData(csock,header);
        break;
    }
}
//���͸�ָ���ͻ��˵�����
int Cellserver::SendData(ClientSock* csock,DataHeader* header){
    if(isRun()&&header){
        return send(csock->getsock(),(const char*)header,header->datalength,0);
    }
    return SOCKET_ERROR;
}
//���͸����пͻ��˵�����
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