#include "myclient.h"

void Myeasyclient::initSocket(){
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
        //printf("建立<Socket=%d>成功...\n",_sock);
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
        printf("错误,连接服务器失败...\n");
    }
    else{
        //printf("建立服务器成功...\n");
    }
    return ret;
}

void Myeasyclient::Close(){
    if(_sock!=INVALID_SOCKET){
#ifdef _WIN32
    closesocket(_sock);
    //清楚windows socket环境
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
            printf("<socket=%d>select ret 任务结束...",_sock);
            return false;
        }
        if(FD_ISSET(_sock,&fdReads)){
            FD_CLR(_sock,&fdReads);
            if(-1==RecvDate(_sock)){
                printf("<socket=%d>select 任务结束...",_sock);
                return false;
            }
        }
        return true;
    }
    return false;
}
//是否在运行中
bool Myeasyclient::isRun(){
    return _sock!=INVALID_SOCKET;
}
//接收数据,处理粘包，拆封包
int Myeasyclient::RecvDate(SOCKET _csock){
    int nlen=(int)recv(_csock,_szRecv,sizeof(_szRecv),0);//取出内容到消息缓存区
    if(nlen<=0){
        printf("与服务器断开连接...\n");
        return -1;
    }
    memcpy(_szMsgBuf+_lastPos,_szRecv,nlen);
    _lastPos+=nlen;
    while(_lastPos>=sizeof(DataHeader)){
        DataHeader* header=(DataHeader*)_szMsgBuf;
        if(_lastPos>=header->datalength){
            int nsize=_lastPos-header->datalength;//剩余未处理的数据长度
            OnNetMsg(header);
            memcpy(_szMsgBuf,_szMsgBuf+_lastPos-nsize,nsize);//将后续数据前移
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
        //printf("收到服务器消息：CMD_LOGIN,数据长度：%d\n", login->datalength);
    }
    break;
    case CMD_LOGIN_RESULT: {
        LoginResult* login = (LoginResult*)header;
        //printf("收到服务器消息：CMD_LOGIN_RESULT,数据长度：%d\n", login->datalength);
    }
    break;
    case CMD_LOGOUT: {
        Logout* login = (Logout*)header;
        //printf("收到服务器消息：CMD_LOGOUT,数据长度：%d\n", login->datalength);
    }
    break;
    
    case CMD_LOGOUT_RESULT:{
        LogoutResult* logout=(LogoutResult*)header;
        //printf("收到服务器消息：CMD_LOGOUT_RESULT,数据长度：%d\n",logout->datalength);
    }
    break;
    case CMD_NEW_USER_JOIN:{
        NewUserJoin* userjoin=(NewUserJoin*)header;
        //printf("收到服务器消息：CMD_NEW_USER_JOIN,数据长度：%d\n",userjoin->datalength);
    }
    break;   
    case CMD_ERROR:{
        printf("收到错误消息,数据长度：%d\n",header->datalength);
    }
    break;
    
    default:
        printf("收到不明消息,数据长度：%d\n",header->datalength);
        break;
    }
}

int Myeasyclient::SendData(DataHeader* header){
    if(isRun()&&header){
        return send(_sock,(const char*)header,header->datalength,0);
    }
    return SOCKET_ERROR;
}