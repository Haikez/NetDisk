#include <myfunc.h>
#include "epollTools.h"
#include "msg.h"
int main(int argc,char *argv[]){
    // ./client ip port
    ARGS_CHECK(argc,3);

    // 用户名和用户路径
    char userName[] = "user1";
    char userPath[2048]={0};
    snprintf(userPath, sizeof(userPath), "%s/: ", userName);

    //ip地址大小端转化
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int ret = connect(sockfd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");
    printf("连接成功！\n");
    write(STDOUT_FILENO,userPath,strlen(userPath));
    int epfd=epoll_create(1);
    epollAdd(epfd,sockfd);
    epollAdd(epfd,STDIN_FILENO);
    while(1){
        struct epoll_event readyEvents[1024];
        int readyEventsLen = epoll_wait(epfd,readyEvents,1024,-1);
        for(int i=0;i<readyEventsLen;++i){
            if(readyEvents[i].data.fd == sockfd){
                // 如果事件来自服务器
                rstMsg_t msg;
                bzero(&msg,sizeof(msg));
                ssize_t bytesRead = recv(sockfd, &msg, sizeof(msg), 0); // 接收服务器消息
                if (bytesRead < 0) {
                    perror("recv");
                    close(epfd);
                    close(sockfd);
                    exit(EXIT_FAILURE);
                } else if(bytesRead>0){
                    if (strcmp(msg.cmd,"cd")==0&&msg.fileExit) {
                        // 如果文件存在，更改客户端的当前目录
                        bzero(userPath,sizeof(userPath));
                        snprintf(userPath, sizeof(userPath), "%s%s: ",userName, msg.userCwd);
                    }
                    if(strcmp(msg.cmd,"exit")==0){
                        printf("Bye bye!!\n");
                        close(epfd);
                        close(sockfd);
                        exit(EXIT_SUCCESS);
                    }
                    if(strlen(msg.rst)){
                        printf("%s\n",msg.rst);
                    }else{
                        write(STDOUT_FILENO,msg.rst,strlen(msg.rst));
                    }
                }else{
                    continue;
                }
                write(STDOUT_FILENO,userPath,strlen(userPath));
            }else if(readyEvents[i].data.fd == STDIN_FILENO){
                char cmd[1024]={0};
                ssize_t cmdLen = read(STDIN_FILENO,cmd,sizeof(cmd));
                send(sockfd,cmd,cmdLen,0);
            }
        }
    }
    return 0;
}
