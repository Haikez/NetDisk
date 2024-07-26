#include <myfunc.h>
typedef struct{
    long len;
    char data[1024];
}train_t;
int transFile(const char *fileName,int netfd){
    train_t train;
    bzero(&train,sizeof(train));
    printf("fileName =%s\n",fileName);
    int fd = open(fileName,O_RDWR);

    // 获取文件长度
    struct stat fstatbuf;
    fstat(fd,&fstatbuf);
    
    off_t fileLen = fstatbuf.st_size;
    bzero(&train,sizeof(train));
    train.len = sizeof(fileLen);
    printf("fileLen = %ld, train.len = %ld\n",fileLen,train.len);
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


