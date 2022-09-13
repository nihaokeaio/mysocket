#include"myserver.h"
#include<thread>

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
    Myeasyserver server;
    //server.InitSocket();
    std::thread t1(cmdthread);
    t1.detach();
    server.Bind(nullptr,1234);
    server.Listen(128);
    server.start();
    while(server.OnRun()&&isrun){
        //server.OnRun();
    }
    server.Close();
    return 0;
}