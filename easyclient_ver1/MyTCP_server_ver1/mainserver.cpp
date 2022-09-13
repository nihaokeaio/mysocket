#include"myserver.h"
#include<thread>

int main(){
    Myeasyserver server;
    //server.InitSocket();
    server.Bind(nullptr,1234);
    server.Listen(5);
    while(server.isRun()){
        server.OnRun();
    }
    return 0;
}