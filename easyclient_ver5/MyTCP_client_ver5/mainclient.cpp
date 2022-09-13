#include"myclient.h"
#include<thread>

#define T_count 4
#define C_count 4000

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

Myeasyclient* client[C_count];
void threadclient(int id){
    int start=(id-1)*(C_count/T_count);
    int end=(id)*(C_count/T_count);
    for(int i=start;i<end;i++){
        if(!isrun){
            return ;
        }
        client[i]=new Myeasyclient();
        
    }
    for(int i=start;i<end;i++){
        if(!isrun){
            return ;
        }
        client[i]->Connect("192.168.58.1",1234);//192.168.137.186
        printf("connect : %d\n",i);
    }
    Login login;
    // strcpy(login.UserName,"zqd");
    // strcpy(login.PassWard,"123321");
    while(isrun){
        for(int i=start;i<end;i++){
            //client[i]->onRun();
            client[i]->SendData(&login);
        }        
    }
    for(int i=start;i<end;i++){
        printf("client socket <%d> exit\n",client[i]->Getsock());
        client[i]->Close();
        delete client[i];
        //client.SendData(&login);
    }
}


int main(){

    

    //printf("hello strat\n");
    //启动线程
    std::thread t1(cmdthread);
    t1.detach();

    std::thread t[T_count];
    for(int i=0;i<T_count;i++){
        t[i]=std::thread(threadclient,i+1);
        t[i].detach();
    }
    while(isrun){
        Sleep(1000);
    }
    printf("已退出...\n");
}