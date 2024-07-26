#ifndef __TASKQUEUE__
#define __TASKQUEUE__
typedef struct node_s {
    int netfd;
    struct node_s *pNext;
} node_t;
typedef struct taskQueue_s {
    node_t *pFront;
    node_t *pRear;
    int qSize;
} taskQueue_t;
void taskQueueInit(taskQueue_t *queue);
void enQueue(taskQueue_t *queue,int netfd);
void deQueue(taskQueue_t *queue);
#endif
