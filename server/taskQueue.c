#include "taskQueue.h"
#include <myfunc.h>
void taskQueueInit(taskQueue_t *queue){
    bzero(queue,sizeof(taskQueue_t));
}
void enQueue(taskQueue_t *queue,int netfd){
    node_t *node = (node_t *)calloc(1,sizeof(node_t));
    node->netfd=netfd;
    if(queue->qSize==0){
        queue->pFront=node;
        queue->pRear=node;
    }else{
        queue->pRear->pNext=node;
        queue->pRear=node;
    }
    ++queue->qSize;
    return;
}
void deQueue(taskQueue_t *queue){
    node_t *node;
    node = queue->pFront;
    if(queue->qSize==1){
        queue->pRear=NULL;
    }
    queue->pFront = node->pNext;
    free(node);
    --queue->qSize;
}
