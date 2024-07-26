#ifndef __HANDLECMD__
#define __HANDLECMD__
#include <myfunc.h>
#include "threadPool.h"
int checkCMD(threadPool_t *threadPool,char *str,int netfd);
int handleCD(threadPool_t *threadPool,char *path,int netfd);
int handlePWD(threadPool_t *threadPool,int netfd);
int handleEXIT(threadPool_t *threadPool,int netfd);
int handleLS(const threadPool_t *threadPool,const char *path,int netfd);
int handlePUTS(const threadPool_t *threadPool,char *fileName,int netfd);
int handleGETS(const threadPool_t *threadPool,char *fileName,int netfd);
int handleRM(const threadPool_t *threadPool,const char *fileName,int netfd);
int handleMKDIR(const threadPool_t *threadPool,const char *dirName,int netfd);
void getFileNameByPath(char *path);
#endif
