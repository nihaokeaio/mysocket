#include"myserver.h"

class MyServer : public Myeasyserver
{
public:

	//只会被一个线程触发 安全
	virtual void OnNetJoin(ClientSock_SMP& pClient)
	{
		Myeasyserver::OnNetJoin(pClient);
	}
	//cellServer 4 多个线程触发 不安全
	
	//如果只开启1个cellServer就是安全的
	virtual void OnNetLeave(ClientSock_SMP& pClient)
	{
		Myeasyserver::ClientleaveOn(pClient);
	}
	//cellServer 4 多个线程触发 不安全
	//如果只开启1个cellServer就是安全的
	virtual void OnNetMsg(Cellserver* pCellServer, ClientSock_SMP& pClient, DataHeader* header)
	{
		Myeasyserver::OnNetMsg(pCellServer, pClient, header);
		switch (header->cmd)
		{
		case CMD_LOGIN:
		{
			//send recv 
			Login* login = (Login*)header;
			//printf("收到客户端<Socket=%d>请求：CMD_LOGIN,数据长度：%d,userName=%s PassWord=%s\n", pClient->sockfd(), login->dataLength, login->userName, login->PassWord);
			//忽略判断用户密码是否正确的过程
			DataHeader_SMP ret = std::make_shared<LoginResult>();
			pCellServer->addSendTask(pClient, ret);
		}//接收 消息---处理 发送   生产者 数据缓冲区  消费者 
		break;
		case CMD_LOGOUT:
		{
			Logout* logout = (Logout*)header;
			//printf("收到客户端<Socket=%d>请求：CMD_LOGOUT,数据长度：%d,userName=%s \n", cSock, logout->dataLength, logout->userName);
			//忽略判断用户密码是否正确的过程
			//LogoutResult ret;
			//SendData(cSock, &ret);
		}
		break;
		default:
		{
			printf("<socket=%d>收到未定义消息,数据长度：%d\n", pClient->getsock(), header->datalength);
		}
		break;
		}
	}
private:

};



bool isrun=true;
void cmdthread(){
    while (true)
    {
        char cmdBuf[256]={};
        scanf("%s",cmdBuf);
        if(strcmp(cmdBuf,"exit")==0){
            isrun=false;
            printf("退出cmdthread线程\n");
            break;
        }
        else {
            printf("不支持命令\n");
        }
    }
    
}


int main(){
	MyServer server;
    //server.InitSocket();
    std::thread t1(cmdthread);
    t1.detach();
    server.Bind(nullptr,1234);
    server.Listen(128);
    server.start(4);
    while(server.OnRun()&&isrun){
        //server.OnRun();
    }
    server.Close();    
    return 0;
}