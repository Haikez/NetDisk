#ifndef __EPOLLTOOLS__
#define __EPOLLTOOLS__
#include <myfunc.h>
int epollAdd(int epfd,int sockfd);
int epollDel(int epfd, int sockfd);
#endif
