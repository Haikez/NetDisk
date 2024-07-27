#include <myfunc.h>
#include "userLogin.h"
void sendLoginMsg(char userName[],int sockfd){
    userLoginMsg_t loginMsg;
    bzero(&loginMsg,sizeof(userLoginMsg_t));
    printf("请输入用户名:");
    scanf("%s",loginMsg.userName);
    char *passwd =getpass("请输入密码:");
    memcpy(&loginMsg.userPasswd,passwd,strlen(passwd));
    send(sockfd,&loginMsg,sizeof(loginMsg),0);
    // 返回用户名
    memcpy(userName,loginMsg.userName,strlen(loginMsg.userName));
}
int recvLoginMsg(char rst[],int sockfd){
    recv(sockfd,rst,1024,0);
    if(strcmp(rst,"验证通过")==0){
        return 0;
    }else{
        return -1;
    }
}
