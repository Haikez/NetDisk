#ifndef __TRANFUN__
#define __TRANFUN__
#include <myfunc.h>
typedef struct{
    long len;
    char data[1024];
}train_t;
int transFile(const char *fileName,int netfd);
int transFileShowProcess(const char *fileName,int netfd);
#endif
