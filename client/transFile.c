#include <myfunc.h>
#include "showProcessBar.h"
typedef struct{
    long len;
    char data[1024];
}train_t;
int transFile(const char *fileName,int netfd){
    train_t train;
    bzero(&train,sizeof(train));
    int fd = open(fileName,O_RDWR);

    // 获取文件长度
    struct stat fstatbuf;
    fstat(fd,&fstatbuf);
    
    off_t fileLen = fstatbuf.st_size;
    bzero(&train,sizeof(train));
    train.len = sizeof(fileLen);
    memcpy(train.data,&fileLen,train.len);
    send(netfd,&train,sizeof(train),MSG_NOSIGNAL);

    while(1){
        ssize_t sret = read(fd,train.data,sizeof(train.data));
        train.len = sret;
        send(netfd,&train,sizeof(train),MSG_NOSIGNAL);
        if(train.len == 0){
            break;
        }
    }
}

// 客户端执行
int transFileShowProcess(const char *fileName,int sockfd){
    train_t train;
    bzero(&train,sizeof(train));
    int fd = open(fileName,O_RDWR);

    // 获取文件长度
    struct stat fstatbuf;
    fstat(fd,&fstatbuf);
    
    off_t fileLen = fstatbuf.st_size;
    off_t currentSize = 0;

    while(1){
        ssize_t sret = read(fd,train.data,sizeof(train.data));
        train.len = sret;
        send(sockfd,&train,sizeof(train),MSG_NOSIGNAL);
        currentSize+=train.len;
        double progress = currentSize*100.0/fileLen;
        show_progress_bar(progress);
        if(train.len == 0){
            break;
        }
        bzero(&train,sizeof(train));
    }
}

