#include"myserver.h"

void Myeasyserver::InitSocket(){
        //��ʼ��������win������ 
#ifdef _WIN32
    WORD ver = MAKEWORD(2,2);
    WSADATA dat;
    WSAStartup(ver,&dat);
#endif
    if(_sock!=INVALID_SOCKET){
        printf("�رվ�����<socket=%d>...\n",_sock);
        Close();
    }
    _sock=socket(AF_INET,SOCK_STREAM,0);
    if(INVALID_SOCKET==_sock){
        printf("����,����Socketʧ��...\n");
    }
    else{
        printf("����<Socket=%d>�ɹ�...\n",_sock);
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
        printf("����,�󶨶˿�<%d>ʧ��...\n",port);
    }
    else{
        printf("�󶨶˿�<%d>�ɹ�...\n",port);
    }
    return ret;
}

int Myeasyserver::Listen(int n){
    int ret=listen(_sock,n);
    if(SOCKET_ERROR == ret){
        printf("Socket=<%d>���󣬼�������ʧ��...",_sock);
    }
    else{
        printf("Socket=<%d>��������ɹ�...",_sock);
    }
    return ret;
}


int Myeasyserver::Accept(){
    sockaddr_in clientAddr={};
    int nAddrLen=sizeof(sockaddr_in);
    SOCKET _csock=INVALID_SOCKET;
#ifdef _WIN32
    _csock=accept(_sock,(sockaddr*)&clientAddr,&nAddrLen);
#else
    _csock=accept(_sock,(sockaddr*)&clientAddr,(socklen_t*)&nAddrLen);
#endif
    if(INVALID_SOCKET==_csock){
        printf("Socket=<%d>���󣬽�����Ч�Ŀͻ�������...\n",_csock);
    }
    else{
        NewUserJoin userJoin;
        SendDataToAll(&userJoin);
        g_clients.push_back(_csock);        
        printf("socket=<%d> �¿ͻ�����,IP = %s\n",(int)_csock,inet_ntoa(clientAddr.sin_addr));
    }
    return _csock;
}

void Myeasyserver::Close(){
    if(_sock!=INVALID_SOCKET){
#ifdef _WIN32
    closesocket(_sock);
    //���windows socket����
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
        fd_set fdWrite;
        fd_set fdExp;

        FD_ZERO(&fdReads);
        FD_ZERO(&fdWrite);
        FD_ZERO(&fdExp);

        FD_SET(_sock,&fdReads);
        FD_SET(_sock,&fdWrite);
        FD_SET(_sock,&fdExp);

        SOCKET maxsock=_sock;
        //
        for(int n=(int)g_clients.size()-1;n>=0;n--){
            FD_SET(g_clients[n],&fdReads);
            if(maxsock<g_clients[n]){
                maxsock=g_clients[n];
            }
        }
        timeval t={0,0};
        int ret =select(maxsock+1,&fdReads,&fdWrite,&fdExp,&t);
        if(ret<0){
            printf("<socket=%d>select �������...\n",_sock);
            Close();
            return false;
        }
        if(FD_ISSET(_sock,&fdReads)){
            FD_CLR(_sock,&fdReads);
            Accept();
        }
        for(int n=(int)g_clients.size()-1;n>=0;n--){
            if(FD_ISSET(g_clients[n],&fdReads)){
                if(-1==RecvDate(g_clients[n])){
                    auto iter=g_clients.begin()+n;
                    if(iter!=g_clients.end()){
                        g_clients.erase(iter);
                    }
                }
            }
        }
        return true;
    }
    return false;
}

bool Myeasyserver::isRun(){
    return _sock!=INVALID_SOCKET;
}


int Myeasyserver::RecvDate(SOCKET _csock){
    int nlen=(int)recv(_csock,_szRecv,sizeof(_szRecv),0);//ȡ��ͷ����Ϣ������short����
    if(nlen<=0){
        printf("��������Ͽ�����...\n");
        return -1;
    }
    memcpy(_szMsgBuf+_lastPos,_szRecv,nlen);
    _lastPos+=nlen;
    while(_lastPos>=sizeof(DataHeader)){
        DataHeader* header=(DataHeader*)_szMsgBuf;
        if(_lastPos>=header->datalength){
            int nsize=_lastPos-header->datalength;//ʣ��δ���������ݳ���
            OnNetMsg(_csock,header);
            memcpy(_szMsgBuf,_szMsgBuf+header->datalength,nsize);//����������ǰ��
        }
        else{
            break;
        }
    }
    return 0;
}

//������Ϣ����
void Myeasyserver::OnNetMsg(SOCKET _csock, DataHeader* header){
    switch (header->cmd)
    {
    case CMD_LOGIN : {
        Login* login = (Login*)header;
        //printf("�յ��ͻ�����Ϣ��CMD_LOGIN,���ݳ��ȣ�%d\n", login->datalength);
    }
    break;
    case CMD_LOGIN_RESULT: {
        LoginResult* login = (LoginResult*)header;
        //printf("�յ��ͻ�����Ϣ��CMD_LOGIN_RESULT,���ݳ��ȣ�%d\n", login->datalength);
    }
    break;
    case CMD_LOGOUT: {
        Logout* login = (Logout*)header;
        //printf("�յ��ͻ�����Ϣ��CMD_LOGOUT,���ݳ��ȣ�%d\n", login->datalength);
    }
    break;
    
    case CMD_LOGOUT_RESULT:{
        LogoutResult* logout=(LogoutResult*)header;
        //printf("�յ��ͻ�����Ϣ��CMD_LOGOUT_RESULT,���ݳ��ȣ�%d\n",logout->datalength);
    }
    break;
    case CMD_NEW_USER_JOIN:{
        NewUserJoin* userjoin=(NewUserJoin*)header;
        //printf("�յ��ͻ�����Ϣ��CMD_NEW_USER_JOIN,���ݳ��ȣ�%d\n",userjoin->datalength);
    }
    break;
    case CMD_ERROR:{
        printf("�յ�������Ϣ,���ݳ��ȣ�%d\n",header->datalength);
    }
    break;    
    default:
        printf("�յ�������Ϣ,���ݳ��ȣ�%d\n",header->datalength);
        break;
    }
    Login log;
    SendData(_csock,&log);
}
//���͸�ָ���ͻ��˵�����
int Myeasyserver::SendData(SOCKET _csock,DataHeader* header){
    if(isRun()&&header){
        return send(_csock,(const char*)header,header->datalength,0);
    }
    return SOCKET_ERROR;
}
//���͸����пͻ��˵�����
void Myeasyserver::SendDataToAll(DataHeader* header){
    for(int n=(int)g_clients.size()-1;n>=0;n--){
        SendData(g_clients[n],header);
    }
}