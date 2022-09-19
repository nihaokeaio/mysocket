#include"myclient.h"
#include<thread>

#define T_count 4
#define Msgcount 1
#define C_count 1000

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
    Myeasyclient::Resetcount();
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
        printf("Thread <%d>,connect : %d\n",id,i);
    }
    std::chrono::milliseconds t(2000);
    std::this_thread::sleep_for(t);
    Login login[Msgcount];
    // strcpy(login.UserName,"zqd");
    // strcpy(login.PassWard,"123321");
    while(isrun){
        for(int i=start;i<end;i++){
            client[i]->onRun();
            //  for(int i=0;i<Msgcount;i++){
            //      if(client[i]->SendData(&login[i])==-1)break;
            //  }         
        }        
    }
    for(int i=start;i<end;i++){
        printf("Thread <%d>, client socket <%d> exit\n",id,client[i]->Getsock());
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
    CELLTimestamp curtime;

    std::thread t[T_count];
    for(int i=0;i<T_count;i++){
        t[i]=std::thread(threadclient,i+1);
        t[i].detach();
    }
    while(isrun){
        auto t1=curtime.getElapsedsecond();
    if(t1>1.0){   
        //std::mutex mux; 
        int msgcount=Myeasyclient::Getcount(); 
        printf("time= %lf,msgcount =%d\n",t1,msgcount);
        Myeasyclient::Resetcount(); 
        curtime.update();
    }
        Sleep(1000);
    }
    printf("已退出...\n");
}