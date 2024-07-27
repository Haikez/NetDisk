#include <myfunc.h>
#include "threadPool.h"
#include "tcpInit.h"
#include "epollTools.h"
#include "dirStack.h"
#include "userLoginCHK.h"
int main(int argc,char *argv[])
{
    // ./server ip port
    ARGS_CHECK(argc,3);
    // 初始化tcp
    int sockfd;
    tcpInit(argv[1],argv[2],&sockfd);
    
    int epfd = epoll_create(1);
    epollAdd(epfd,sockfd);

    // 创建线程池
    threadPool_t threadPool;
    threadPoolInit(&threadPool,3);
    //threadPool.userName = "user1";
    //threadPool.user_cwd="user1/";
    while(1){
        struct epoll_event readyEvents[1024];
        int readyEventsLen = epoll_wait(epfd,readyEvents,1024,-1);
        for(int i=0;i<readyEventsLen;++i){
            if(readyEvents[i].data.fd == sockfd){
                int netfd = accept(sockfd,NULL,NULL);
                // 验证用户登录
                user_t user;
                userLoginMsg_t loginMsg;
                bzero(&user,sizeof(user_t));
                bzero(&loginMsg,sizeof(userLoginMsg_t));
                if(checkLoginMsg(&loginMsg,netfd)==0){
                    //初始化一个用户
                    user.netfd = netfd;
                    dirStackInit(&user.dirStack);
                    dirPush(&user.dirStack,"/"); //用户当前目录默认为"/"
                    memcpy(user.userCwd,"/",1);
                    memcpy(user.userName,loginMsg.userName,strlen(loginMsg.userName));
                    printf("登录成功！\n");
                }else{
                    printf("登录失败！\n");
                    continue;
                }
                // 将得到的netfd加入线程池的任务队列
                pthread_mutex_lock(&threadPool.mutex);
                // 将新连接的用户加入用户表中
                userArrAdd(&threadPool.userArr,user);
                printf("用户%s已上线\n",user.userName);
                // 将新连接的用户加入threadepoll中
                epollAdd(threadPool.threadepfd,netfd);
                pthread_cond_signal(&threadPool.cond);
                pthread_mutex_unlock(&threadPool.mutex);
                
            }
        }

    }
    return 0;
}

