#include "threadPool.h"
#include "worker.h"
int threadPoolInit(threadPool_t *threadPool,int workNum){
    threadPool->workNum=workNum;
    threadPool->threadepfd = epoll_create(1);
    threadPool->home="/home/parallels/Desktop/Linux/NetDisk/server/netDisk/";
    userArrInit(&threadPool->userArr);
    makeWorker(threadPool);
}
