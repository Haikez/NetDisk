#include "userLoginCHK.h"
#include <myfunc.h>
// 验证密码的正确性 同时返回用户的登录信息
int checkLoginMsg(userLoginMsg_t *loginMsg,int netfd){
    // 收到用户的登录信息
    char loginRst[1024]={0};
    bzero(loginMsg,sizeof(userLoginMsg_t));
    recv(netfd,loginMsg,sizeof(userLoginMsg_t),0);
    struct spwd *sp;
    char salt[512]={0};//盐值
    if((sp=getspnam(loginMsg->userName))==NULL){
        memcpy(loginRst,"用户不存在",strlen("用户不存在"));
        send(netfd,loginRst,strlen(loginRst),0);
        return -1;
    }
    // 得到盐值
     int i,j;
    for(i=0,j=0;sp->sp_pwdp[i]&&j!=4;++i){
        if(sp->sp_pwdp[i]=='$'){
            ++j;
        }
    }
    strncpy(salt,sp->sp_pwdp,i-1);
    // 进行密码验证
    char *genPwd=crypt(loginMsg->userPasswd,salt);
    if(strcmp(sp->sp_pwdp,genPwd)==0)
    {
        memcpy(loginRst,"验证通过",sizeof("验证通过"));
        send(netfd,loginRst,sizeof(loginRst),0);
        return 0;
    }else{
        memcpy(loginRst,"验证失败",sizeof("验证失败"));
        send(netfd,loginRst,sizeof(loginRst),0);
        return -1;
    }
}
