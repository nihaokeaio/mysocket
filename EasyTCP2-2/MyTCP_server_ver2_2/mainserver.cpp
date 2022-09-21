#include"myserver.h"

class MyServer : public Myeasyserver
{
public:

	//ֻ�ᱻһ���̴߳��� ��ȫ
	virtual void OnNetJoin(ClientSock_SMP& pClient)
	{
		Myeasyserver::OnNetJoin(pClient);
	}
	//cellServer 4 ����̴߳��� ����ȫ
	
	//���ֻ����1��cellServer���ǰ�ȫ��
	virtual void OnNetLeave(ClientSock_SMP& pClient)
	{
		Myeasyserver::ClientleaveOn(pClient);
	}
	//cellServer 4 ����̴߳��� ����ȫ
	//���ֻ����1��cellServer���ǰ�ȫ��
	virtual void OnNetMsg(Cellserver* pCellServer, ClientSock_SMP& pClient, DataHeader* header)
	{
		Myeasyserver::OnNetMsg(pCellServer, pClient, header);
		switch (header->cmd)
		{
		case CMD_LOGIN:
		{
			//send recv 
			Login* login = (Login*)header;
			//printf("�յ��ͻ���<Socket=%d>����CMD_LOGIN,���ݳ��ȣ�%d,userName=%s PassWord=%s\n", pClient->sockfd(), login->dataLength, login->userName, login->PassWord);
			//�����ж��û������Ƿ���ȷ�Ĺ���
			DataHeader_SMP ret = std::make_shared<LoginResult>();
			pCellServer->addSendTask(pClient, ret);
		}//���� ��Ϣ---���� ����   ������ ���ݻ�����  ������ 
		break;
		case CMD_LOGOUT:
		{
			Logout* logout = (Logout*)header;
			//printf("�յ��ͻ���<Socket=%d>����CMD_LOGOUT,���ݳ��ȣ�%d,userName=%s \n", cSock, logout->dataLength, logout->userName);
			//�����ж��û������Ƿ���ȷ�Ĺ���
			//LogoutResult ret;
			//SendData(cSock, &ret);
		}
		break;
		default:
		{
			printf("<socket=%d>�յ�δ������Ϣ,���ݳ��ȣ�%d\n", pClient->getsock(), header->datalength);
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
            printf("�˳�cmdthread�߳�\n");
            break;
        }
        else {
            printf("��֧������\n");
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