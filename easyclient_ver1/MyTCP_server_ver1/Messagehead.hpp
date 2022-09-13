#ifndef _MESSAGEHEADR_H
#define _MESSAGEHEADR_H
enum CMD{
    CMD_LOGIN,
    CMD_LOGIN_RESULT,
    CMD_LOGOUT,
    CMD_LOGOUT_RESULT,
    CMD_NEW_USER_JOIN,
    CMD_ERROR
};

struct DataHeader{
    short datalength=0;
    short cmd=5;
};

struct Login: public DataHeader
{
    Login(){
        datalength=sizeof(Login);
        cmd=CMD_LOGIN;
    }
    char UserName[32];
    char PassWard[32];
    char p[1024];
};

struct LoginResult: public DataHeader
{
    LoginResult(){
        datalength=sizeof(LoginResult);
        cmd=CMD_LOGIN_RESULT;
        res=0;
    }
    int res;
};

struct Logout: public DataHeader
{
    Logout(){
        datalength=sizeof(Logout);
        cmd=CMD_LOGOUT;
    }
    char UserName[32];
};

struct LogoutResult: public DataHeader
{
    LogoutResult(){
        datalength=sizeof(LogoutResult);
        cmd=CMD_LOGOUT_RESULT;
        res=0;
    }
    int res;
};

struct NewUserJoin: public DataHeader
{
    NewUserJoin(){
        datalength=sizeof(NewUserJoin);
        cmd= CMD_NEW_USER_JOIN;
        sock=0;
    }
    int sock;
};

#endif

