#include"myclient.h"
#include<thread>
//sudo tcpdump -i 1 host 192.168.58.128 and tcp port 1234 //TCPץ��
void cmdthread(Myeasyclient* client){
    while (true)
    {
        char cmdBuf[256]={};
        scanf("%s",cmdBuf);
        if(strcmp(cmdBuf,"exit")==0){
            client->Close();
            printf("�˳�cmdthread�߳�\n");
        }
        else if(strcmp(cmdBuf,"login")==0){
            Login login;
            strcpy(login.UserName,"alias");
            strcpy(login.PassWard,"nihao");
            client->SendData(&login);
        }
        else if(strcmp(cmdBuf,"logout")==0){
            Logout logout;
            strcpy(logout.UserName,"lyd");
            client->SendData(&logout);
        }
        else {
            printf("��֧������\n");
        }
    }
    
}

int main(){
    Myeasyclient client;
    client.initSocket();
    client.Connect("192.168.1.102",1234);//192.168.137.186
    //printf("hello strat\n");
    //�����߳�
    //std::thread t1(cmdthread,&client);
    //t1.detach();
    Login login;
    strcpy(login.UserName,"zqd");
    strcpy(login.PassWard,"123321");
    while(client.isRun()){
        client.onRun();
        client.SendData(&login);
    }
    client.Close();
    printf("���˳�...\n");
    getchar();
}