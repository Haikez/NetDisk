#include <myfunc.h>
#include "login.h"
void sendLoginMsg(int sockfd){
    userLoginMsg_t loginMsg;
    bzero(&loginMsg,sizeof(userLoginMsg_t));
    printf("请输入用户名:");
    scanf("%s",loginMsg.userName);
    printf("请输入密码:");
    scanf("%s",loginMsg.userPasswd);
    printf("\n");
    printf("%s\n",loginMsg.userName);
    printf("%s\n",loginMsg.userPasswd);

    //send(sockfd,&loginMsg,sizeof(loginMsg),0);
}
