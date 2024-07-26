#ifndef __USERARR__
#define __USERARR__
#include "dirStack.h"
typedef struct user_s{
    // 用户名
    char userName[1024];
    // 用户的netfd;
    int netfd;
    // 用户的工作目录
    char userCwd[1024];
    // 用户当前目录栈
    dirStack_t dirStack;
    struct user_s *pNext;
} user_t;
typedef struct userArr_s{
    // 用户列表
    user_t *pHead;
    user_t *pTail;
    // 记录长度
    int size;
} userArr_t;
 void userArrInit(userArr_t *userArr);
 void userArrAdd(userArr_t *userArr,user_t user);
 void userArrDelByFd(userArr_t *userArr,int netfd);
 user_t *getUserByFd(const userArr_t *userArr,int netfd);
#endif
