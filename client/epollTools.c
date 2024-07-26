#include "epollTools.h"
int epollAdd(int epfd,int sockfd){
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&event);
    return 0;
}
int epollDel(int epfd, int sockfd){
    int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
    ERROR_CHECK(ret,-1,"epollDel");
    return 0;
}



