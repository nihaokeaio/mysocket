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
    DataHeader(){
        datalength=sizeof(DataHeader);
        cmd=CMD_ERROR;
    }
    short datalength;
    short cmd;
};

struct Login: public DataHeader
{
    Login(){
        datalength=sizeof(Login);
        cmd=CMD_LOGIN;
    }
    char UserName[32];
    char PassWard[32];
    //char p[956];
};

struct LoginResult: public DataHeader
{
    LoginResult(){
        datalength=sizeof(LoginResult);
        cmd=CMD_LOGIN_RESULT;
        res=0;
    }
    int res;
    //char p[1016];
};

struct Logout: public DataHeader
{
    Logout(){
        datalength=sizeof(Logout);
        cmd=CMD_LOGOUT;
    }
    char UserName[32];
    //char p[988];
};

struct LogoutResult: public DataHeader
{
    LogoutResult(){
        datalength=sizeof(LogoutResult);
        cmd=CMD_LOGOUT_RESULT;
        res=0;
    }
    int res;
    //char p[1016];
};

struct NewUserJoin: public DataHeader
{
    NewUserJoin(){
        datalength=sizeof(NewUserJoin);
        cmd=CMD_LOGOUT_RESULT;
        sock=0;
    }
    int sock;
    //char p[1016];
};

#endif

