#include "myclient.h"

void Myeasyclient::initSocket(){
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
        //printf("����<Socket=%d>�ɹ�...\n",_sock);
    }
}



int Myeasyclient::Connect(const char* ip,unsigned short port){
    if(_sock==INVALID_SOCKET){
        initSocket();
    }
    sockaddr_in _sin={};
    _sin.sin_family=AF_INET;
    _sin.sin_port=htons(port);
#ifdef _WIN32
    _sin.sin_addr.S_un.S_addr=inet_addr(ip);
#else
    _sin.sin_addr.s_addr=inet_addr(ip);
#endif
    int ret=connect(_sock,(sockaddr*)&_sin,sizeof(sockaddr_in));
    //printf("res=%d\n",ret);
    if(SOCKET_ERROR==ret){
        printf("����,���ӷ�����ʧ��...\n");
    }
    else{
        //printf("�����������ɹ�...\n");
    }
    return ret;
}

void Myeasyclient::Close(){
    if(_sock!=INVALID_SOCKET){
#ifdef _WIN32
    closesocket(_sock);
    //���windows socket����
    WSACleanup();
#else
    Close();
#endif
    _sock=INVALID_SOCKET;
    }
}

bool Myeasyclient::onRun(){
    if(isRun()){
        fd_set fdReads;
        FD_ZERO(&fdReads);
        FD_SET(_sock,&fdReads);
        timeval t={0,0};
        int ret=select(_sock+1,&fdReads,0,0,&t);
        if(ret<0){
            printf("<socket=%d>select ret �������...",_sock);
            return false;
        }
        if(FD_ISSET(_sock,&fdReads)){
            FD_CLR(_sock,&fdReads);
            if(-1==RecvDate(_sock)){
                printf("<socket=%d>select �������...",_sock);
                return false;
            }
        }
        return true;
    }
    return false;
}
//�Ƿ���������
bool Myeasyclient::isRun(){
    return _sock!=INVALID_SOCKET;
}
//��������,����ճ��������
int Myeasyclient::RecvDate(SOCKET _csock){
    int nlen=(int)recv(_csock,_szRecv,sizeof(_szRecv),0);//ȡ�����ݵ���Ϣ������
    if(nlen<=0){
        printf("��������Ͽ�����...\n");
        return -1;
    }
    memcpy(_szMsgBuf+_lastPos,_szRecv,nlen);
    _lastPos+=nlen;
    while(_lastPos>=sizeof(DataHeader)){
        DataHeader* header=(DataHeader*)_szMsgBuf;
        if(_lastPos>=header->datalength){
            int nsize=_lastPos-header->datalength;//ʣ��δ��������ݳ���
            OnNetMsg(header);
            memcpy(_szMsgBuf,_szMsgBuf+_lastPos-nsize,nsize);//����������ǰ��
            _lastPos=nsize;
        }
        else{
            break;
        }
    }
    return 0;
}

void Myeasyclient::OnNetMsg(DataHeader* header){
    switch (header->cmd)
    {
    case CMD_LOGIN : {
        Login* login = (Login*)header;
        //printf("�յ���������Ϣ��CMD_LOGIN,���ݳ��ȣ�%d\n", login->datalength);
    }
    break;
    case CMD_LOGIN_RESULT: {
        LoginResult* login = (LoginResult*)header;
        //printf("�յ���������Ϣ��CMD_LOGIN_RESULT,���ݳ��ȣ�%d\n", login->datalength);
    }
    break;
    case CMD_LOGOUT: {
        Logout* login = (Logout*)header;
        //printf("�յ���������Ϣ��CMD_LOGOUT,���ݳ��ȣ�%d\n", login->datalength);
    }
    break;
    
    case CMD_LOGOUT_RESULT:{
        LogoutResult* logout=(LogoutResult*)header;
        //printf("�յ���������Ϣ��CMD_LOGOUT_RESULT,���ݳ��ȣ�%d\n",logout->datalength);
    }
    break;
    case CMD_NEW_USER_JOIN:{
        NewUserJoin* userjoin=(NewUserJoin*)header;
        //printf("�յ���������Ϣ��CMD_NEW_USER_JOIN,���ݳ��ȣ�%d\n",userjoin->datalength);
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
}

int Myeasyclient::SendData(DataHeader* header){
    if(isRun()&&header){
        return send(_sock,(const char*)header,header->datalength,0);
    }
    return SOCKET_ERROR;
}