#include "userArr.h"
#include <myfunc.h>
void userArrInit(userArr_t *userArr){
    userArr = (userArr_t *)malloc(sizeof(userArr_t));
    bzero(userArr,sizeof(userArr_t));
}
void userArrAdd(userArr_t *userArr,user_t user){
    user_t *pUser = (user_t *)malloc(sizeof(user_t));
    memcpy(pUser,&user,sizeof(user));

    //pUser->netfd=user.netfd;
    //pUser->userCwd=user.userCwd;
    //pUser->userName = user.userName;
    //dirStackInit(&pUser->dirStack);
    //dirPush(&pUser->dirStack,"/"); //用户当前目录默认为"/"

    if(userArr->size==0){
        userArr->pHead = pUser;
        userArr->pTail= pUser;
        pUser->pNext = NULL;
    }else{
        userArr->pTail->pNext=pUser;
        userArr->pTail=pUser;
    }
    userArr->size++;
}
void userArrDelByFd(userArr_t *userArr,int netfd){
    user_t *p=userArr->pHead;
    user_t *r=p;
    if(p==NULL) return;
    while(p->pNext){
        if(p->pNext->netfd==netfd){
            r=p;
            p=p->pNext;
            r->pNext=p->pNext;
            break;
        }
    }
    free(p);
}
 user_t *getUserByFd(const userArr_t *userArr,int netfd){
    user_t *p=userArr->pHead;
    while(p){
        if(p->netfd==netfd){
            return p;
        }
    }
    return NULL;
 }
