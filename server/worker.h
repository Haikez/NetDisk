#ifndef __WORKER__
#define __WORKER__
#include <myfunc.h>
#include "threadPool.h"
void *threadFunc(void * arg);
int makeWorker(threadPool_t *threadPool);

#endif
