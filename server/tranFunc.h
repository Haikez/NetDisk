#ifndef __TRANFUN__
#define __TRANFUN__
#include <myfunc.h>
typedef struct{
    long len;
    char data[1024];
}train_t;
int tranFunc(int netfd);
#endif
