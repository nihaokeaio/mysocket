#include"myclient.h"
#include<thread>
//sudo tcpdump -i 1 host 192.168.58.128 and tcp port 1234 //TCP抓包
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
    const int ccount=1000;
    Myeasyclient* client[ccount];
    for(int i=0;i<ccount;i++){
        client[i]=new Myeasyclient();
        client[i]->initSocket();
        client[i]->Connect("192.168.58.128",1234);//192.168.137.186
    }
    
    //printf("hello strat\n");
    //启动线程
    std::thread t1(cmdthread);
    t1.detach();
    Login login;
    strcpy(login.UserName,"zqd");
    strcpy(login.PassWard,"123321");
    while(isrun){
        for(int i=0;i<ccount;i++){
            //client[i]->onRun();
            client[i]->SendData(&login);
        }        
    }
    for(int i=0;i<ccount;i++){
            client[i]->Close();
            delete client[i];
            //client.SendData(&login);
        }   
    printf("已退出...\n");
}