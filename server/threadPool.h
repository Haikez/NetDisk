#ifndef __THREADPOOL__
#define __THREADPOOL__
#include<myfunc.h>
#include "userArr.h"
typedef struct tidArr_s {
    int workNum;
    pthread_t *arr;
} tidArr_t;
typedef struct threadPool_s{
    // 线程id列表
    tidArr_t tidArr;
    // 用户列表
    userArr_t userArr;
    // 工人数
    int workNum;
    // 锁
    pthread_mutex_t mutex;
    // 条件变量
    pthread_cond_t cond;
    // 家目录
    char *home;
    // threadepfd;
    int threadepfd;

} threadPool_t;
int threadPoolInit(threadPool_t *threadPool,int workNum);
#endif
