#ifndef __LOGIN__
#define __LOGIN__
typedef struct userLoginMsg_s{
    char userName[1024];
    char userPasswd[1024];
}userLoginMsg_t;
// 接收并发送用户登录信息给服务端
void sendLoginMsg(char userName[],int sockfd);
// 收到用户的登录信息，并打印到控制台
int recvLoginMsg(char rst[],int sockfd);
#endif
