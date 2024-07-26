#include "dirStack.h"
void dirStackInit(dirStack_t *dirStack){
    bzero(dirStack,sizeof(dirStack_t));
    dirStack->top=-1;
}
int dirPush(dirStack_t *dirStack,char *dirName){
    char * pdirName = (char*)malloc(strlen(dirName));
    memcpy(pdirName,dirName,strlen(dirName));
    // 处理目录名，确保把dir/存入栈
    while(pdirName[strlen(pdirName)-1]=='/'){
        pdirName[strlen(pdirName)-1]='\0';
    }
    strcat(pdirName,"/");
    if(dirStack->top<=1023){
        dirStack->top++;
        dirStack->dirName[dirStack->top]=pdirName;
        return 1;
    }else{
        return 0;
    }
}
int dirPop(dirStack_t *dirStack){
    char *p = dirStack->dirName[dirStack->top];
    if(dirStack->top!=-1){
        --dirStack->top;
        free(p);
        return 1;
    }else{
        return 0;
    }
}
char *getCwd(dirStack_t *dirStack){
    char *path = (char *)malloc(1024);
    int k=0;
    for(int i=0;i<=dirStack->top;i++){
        for(int j=0;dirStack->dirName[i][j]!='\0';++j){
            path[k++]=dirStack->dirName[i][j];
        }
    }
    return path;
}
int dirPushPath(dirStack_t *dirStack,char *pathName){
    char *each, *pstr;
    pstr = pathName;
    dirStackInit(dirStack);
    char buf[1024]={0};
    if(pathName[0]=='/'){ 
        // 如果输入的是绝对路径 
        // 清空栈                                         
        bzero(dirStack->dirName,sizeof(dirStack->dirName));
        dirStack->top=-1;
        dirPush(dirStack,"/"); 
    }
    while((each = strtok(pstr,"/"))!=NULL){
        sprintf(buf,"%s/",each);
        dirPush(dirStack,buf);
        bzero(buf,sizeof(buf));
        pstr=NULL;
    }
}
