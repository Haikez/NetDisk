#include "dirStack.h"
void dirStackInit(dirStack_t *dirStack){
    bzero(dirStack,sizeof(dirStack_t));
    dirStack->top=-1;
}
int dirPush(dirStack_t *dirStack,char *dirName){
    char * pdirName = (char*)malloc(1024);
    bzero(pdirName,1024);
    memcpy(pdirName,dirName,strlen(dirName));
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
void getCwd(dirStack_t *dirStack,char *userCwd){
    bzero(userCwd,sizeof(userCwd));
    int k=0;
    for(int i=0;i<=dirStack->top;i++){
        for(int j=0;dirStack->dirName[i][j]!='\0';++j){
            userCwd[k++]=dirStack->dirName[i][j];
        }
    }
}
int dirPushPath(dirStack_t *dirStack,char *pathName){
    char *each, *pstr;
    pstr = pathName;
    char buf[1024]={0};
    
    if(pathName[0]=='/'){ 
        // 如果输入的是绝对路径 
        // 清空栈                                         
        bzero(dirStack->dirName,sizeof(dirStack->dirName));
        dirStack->top=-1;
        dirPush(dirStack,"/"); 
    }
    while((each = strtok(pstr,"/"))!=NULL){
        pstr=NULL;
        if(strcmp(each,"..")==0){
            // 如果当前目录名是“..”说明要出栈一个目录，即从上一层开始
            if(dirStack->top<=0){
                return -1;
            }      
            // 用户当前目录栈出队
            dirPop(dirStack);
            continue;
        }
        bzero(buf,sizeof(buf));
        sprintf(buf,"%s/",each);
        dirPush(dirStack,buf);
    }
    return 0;

}
