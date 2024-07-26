#include "worker.h"
#include "threadPool.h"
#include "epollTools.h"
#include "handleCMD.h"
void unlock(void *arg){
    pthread_mutex_t *mutex = (pthread_mutex_t *)arg;
    pthread_mutex_unlock(mutex);
}
void *threadFunc(void * arg){
    threadPool_t *threadPool = (threadPool_t *)arg;
    while(1){
        pthread_mutex_lock(&threadPool->mutex);
        pthread_cleanup_push(unlock,&threadPool->mutex);
        while(threadPool->userArr.size<=0){
            pthread_cond_wait(&threadPool->cond,&threadPool->mutex);
        }
        //deQueue(&threadPool->taskArr);
        pthread_cleanup_pop(1);
    
        struct epoll_event readyEvents[1024];
        int readyEventsLen = epoll_wait(threadPool->threadepfd,readyEvents,1024,-1);
        for(int i=0;i<readyEventsLen;++i){
            char cmd[1024]={0};
            ssize_t cmdLen = recv(readyEvents[i].data.fd,cmd,1024,0);
            cmd[cmdLen-1]='\0';
            //printf("cmd[%ld]: %s\n",strlen(cmd),cmd);
            checkCMD(threadPool,cmd,readyEvents[i].data.fd);
        }
        //close(netfd);
    } 
}
int makeWorker(threadPool_t *threadPool){
    threadPool->tidArr.arr = (pthread_t *)calloc(threadPool->workNum,sizeof(pthread_t));
    for(int i=0;i<threadPool->workNum;i++){
        pthread_create(&threadPool->tidArr.arr[i],NULL,threadFunc,threadPool);
    }
}
