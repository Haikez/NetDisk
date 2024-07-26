#ifndef __DIRSTACK__
#define __DIRSTACK__
#include <myfunc.h>
typedef struct dirStack_s{
    char *dirName[1024];
    int top;
}dirStack_t;

void dirStackInit(dirStack_t *dirStack);
int dirPush(dirStack_t *dirStack,char *dirName);
int dirPop(dirStack_t *dirStack);
void getCwd(dirStack_t *dirStack,char *userCwd);
int dirPushPath(dirStack_t *dirStack,char *pathName);
#endif
