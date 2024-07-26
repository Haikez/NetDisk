#include "handleCMD.h"
#include "epollTools.h"
#include "msg.h"
#include "transFile.h"
int checkCMD(threadPool_t *threadPool,char *str,int netfd){
    char *cmd,*path;
    cmd = strtok(str," ");
    path = strtok(NULL," ");

    rstMsg_t msg;
    bzero(&msg,sizeof(msg));
    if(strlen(str)==0){
        bzero(&msg,sizeof(msg));
        send(netfd,&msg,sizeof(msg),0);
    }else if(strcmp(cmd,"cd")==0){
        if(path==NULL){
            send(netfd,&msg,sizeof(msg),0);
            return 0;
        }
        handleCD(threadPool,path,netfd);
    }else if(strcmp(cmd,"ls")==0){
        if(path==NULL){
            path=".";
        }
        handleLS(threadPool,path, netfd);
    }else if(strcmp(cmd,"pwd")==0){
        handlePWD(threadPool,netfd);
    }else if(strcmp(cmd,"puts")==0){
        handlePUTS(threadPool,path,netfd);
    }else if(strcmp(cmd,"gets")==0){
        handleGETS(threadPool,path,netfd);
    }else if(strcmp(cmd,"rm")==0){
        handleRM(threadPool,path,netfd);
    }else if(strcmp(cmd,"mkdir")==0){
        handleMKDIR(threadPool,path,netfd);
    }else if(strcmp(cmd,"exit")==0){
        handleEXIT(threadPool,netfd);
    }else{
        bzero(&msg,sizeof(msg));
        send(netfd,&msg,sizeof(msg),0);
    }
}
int handlePUTS(const threadPool_t *threadPool,char *fileName,int netfd){
    rstMsg_t msg;
    bzero(&msg,sizeof(msg));
    memcpy(msg.cmd,"puts",sizeof("puts"));
    memcpy(msg.rst,fileName,strlen(fileName));
    send(netfd,&msg,sizeof(msg),0);

    train_t train;
    bzero(&train,sizeof(train));

    char absolutePath[2048]={0};
    user_t *user = getUserByFd(&threadPool->userArr,netfd);
    sprintf(absolutePath,"%s%s%s%s",threadPool->home,user->userName,user->userCwd,fileName);

    int fd = open(absolutePath,O_RDWR|O_TRUNC|O_CREAT,0666);
    while(1){
        recv(netfd,&train,sizeof(train),MSG_WAITALL);
        if(train.len == 0){
            memcpy(msg.rst,"上传完成!OK",strlen("上传完成!OK"));
            send(netfd,&msg,sizeof(msg),0);
            break;
        }
        write(fd,train.data,train.len);
        bzero(&train,sizeof(train));
    }
}

int handleGETS(const threadPool_t *threadPool,char *fileName,int netfd){
    rstMsg_t msg;
    bzero(&msg,sizeof(msg));
    memcpy(msg.cmd,"gets",sizeof("gets"));

    char absolutePath[2048]={0};
    user_t *user = getUserByFd(&threadPool->userArr,netfd);
    if(fileName==NULL){
        msg.fileExit=0;
        send(netfd,&msg,sizeof(msg),0);
        return -1;
    }else if(fileName[0]=='/'){
        // 如果输入的是绝对路径
        sprintf(absolutePath,"%s%s%s",threadPool->home,user->userName,fileName);
    }else{
        sprintf(absolutePath,"%s%s%s%s",threadPool->home,user->userName,user->userCwd,fileName);
    }
    // 根据绝对路径查询是否存在
    int ret = access(absolutePath,F_OK);
    if(ret != 0){ // 路径不存在
        char *rst=strerror(errno);
        memcpy(msg.rst,rst,strlen(rst));
        msg.fileExit=0;
        send(netfd,&msg,sizeof(msg),0);
        return -1;
    }
    msg.fileExit=1;
    getFileNameByPath(fileName);
    memcpy(msg.rst,fileName,strlen(fileName));
    send(netfd,&msg,sizeof(msg),0);
    transFile(absolutePath,netfd);

}
void getFileNameByPath(char *path){
    char *each;
    char *str=path;
    char *last;
    while((each = strtok(str,"/"))!=NULL){
        str=NULL;
        last=each;
    }
    path=last;
}

int handleRM(const threadPool_t *threadPool,const char *fileName,int netfd){
    rstMsg_t msg;
    bzero(&msg,sizeof(msg));
    memcpy(msg.cmd,"rm",sizeof("rm"));

    char absolutePath[2048]={0};
    user_t *user = getUserByFd(&threadPool->userArr,netfd);
    if(fileName==NULL){
        msg.fileExit=0;
        send(netfd,&msg,sizeof(msg),0);
        return -1;
    }else if(fileName[0]=='/'){
        // 如果输入的是绝对路径
        sprintf(absolutePath,"%s%s%s",threadPool->home,user->userName,fileName);
    }else{
        sprintf(absolutePath,"%s%s%s%s",threadPool->home,user->userName,user->userCwd,fileName);
    }
    int rst =unlink(absolutePath);
    if(rst==-1){
        char *rst=strerror(errno);
        memcpy(msg.rst,rst,strlen(rst));
        msg.fileExit=0;
        send(netfd,&msg,sizeof(msg),0);
        printf("删除失败\n");
    }else{
        printf("删除成功\n");
        msg.fileExit=1;
        send(netfd,&msg,sizeof(msg),0);
    }
}

int handleMKDIR(const threadPool_t *threadPool,const char *dirName,int netfd){
    rstMsg_t msg;
    bzero(&msg,sizeof(msg));
    memcpy(msg.cmd,"mkdir",sizeof("mkdir"));

    char absolutePath[2048]={0};
    user_t *user = getUserByFd(&threadPool->userArr,netfd);
    if(dirName==NULL){
        // 如果输入的文件夹名为空
        msg.fileExit=0;
        send(netfd,&msg,sizeof(msg),0);
        return -1;
    }else if(dirName[0]=='/'){
        // 如果输入的是绝对路径
        sprintf(absolutePath,"%s%s%s",threadPool->home,user->userName,dirName);
    }else{
        sprintf(absolutePath,"%s%s%s%s",threadPool->home,user->userName,user->userCwd,dirName);
    }
    int rst = mkdir(absolutePath,0775);
    if(rst==-1){
        char *rst=strerror(errno);
        memcpy(msg.rst,rst,strlen(rst));
        msg.fileExit=0;
        send(netfd,&msg,sizeof(msg),0);
        printf("创建失败\n");
    }else{
        printf("创建成功\n");
        msg.fileExit=1;
        send(netfd,&msg,sizeof(msg),0);
    }
}
int handleLS(const threadPool_t *threadPool,const char *path,int netfd){
    rstMsg_t msg;
    bzero(&msg,sizeof(msg));

    // 获取用户的当前目录
    char *userName;
    user_t *user = getUserByFd(&threadPool->userArr,netfd);
    userName = user->userName;
    char absolutePath[2048]={0};
    if(path[0]=='/'){
        // 如果用户输入绝对路径
        sprintf(absolutePath,"%s%s%s",threadPool->home,userName,path);
    }else{
        sprintf(absolutePath,"%s%s%s%s",threadPool->home,userName,user->userCwd,path);
    }

    // 根据绝对路径查询是否存在
    //int ret = access(absolutePath,F_OK);
    //if(ret != 0){ // 路径不存在
    //    msg.fileExit=0;
    //    return -1;
    //}
    DIR * dir=opendir(absolutePath);
    if(!dir){
        // 文件夹不存在
        bzero(&msg,sizeof(msg));
        msg.fileExit=0;
        char *rst=strerror(errno);
        memcpy(msg.rst,rst,strlen(rst));
        send(netfd,&msg,sizeof(msg),0);
        return -1;
    }
    struct dirent * pdir;

    // 临时缓冲区，收集每一行的输出
    char buf[2048]={0};
    int offset = 0;
    while(pdir=readdir(dir)){
        struct stat filestat;
        //获取文件详细信息
        int ret=stat(absolutePath,&filestat);
        ERROR_CHECK(ret,-1,"stat");
        //提取文件类型
        char file_type;
        switch (filestat.st_mode & S_IFMT) {
        case S_IFBLK:  file_type='b';      break;
        case S_IFCHR:  file_type='c';      break;
        case S_IFDIR:  file_type='d';      break;
        case S_IFIFO:  file_type='f';      break;
        case S_IFLNK:  file_type='l';      break;
        case S_IFREG:  file_type='-';      break;
        case S_IFSOCK: file_type='s';      break;
        default:       file_type='-';      break;
        }
        //获取文件权限
        int filePerm=filestat.st_mode&0777;
        char *PermList[8]={"---","--x","-w-","-wx","r--","r-x","rw-","rwx"};
        int usrPerm=(filePerm&0700)>>6;
        int grpPerm=(filePerm&0070)>>3;
        int othPerm=filePerm&0007;
        //printf("%o %o %o\n",usrPerm,grpPerm,othPerm);
        //拼接文件权限字符串
        char filePermStr[1024]={0};
        sprintf(filePermStr,"%s%s%s",PermList[usrPerm],PermList[grpPerm],PermList[othPerm]);
        // printf("%s\n",filePermStr);

        //获取文件所属的用户名
        struct passwd * passwd =getpwuid( filestat.st_uid);
        ERROR_CHECK(passwd,NULL,"getpwuid");


        //获取组名
        struct group *grp=getgrgid(filestat.st_gid);
        ERROR_CHECK(grp,NULL,"getgrgid");

        //获取修改时间struct timespec st_mtim;
        //printf("%ld",filestat.st_mtime);
        //根据时间戳，转化为自定义时间
        struct tm *timeRet=localtime(&filestat.st_mtime);
        ERROR_CHECK(timeRet,NULL,"localtime");

        //拼接月份字符串
        const char *months[] = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };

        bzero(buf,sizeof(buf));
        sprintf(buf,"%c%s %d %s %s %8ld %s %d %2d:%2d %s\n",
                file_type,filePermStr,
                filestat.st_nlink,
                passwd->pw_name,
                grp->gr_name,
                filestat.st_size,
                months[timeRet->tm_mon],
                timeRet->tm_mday,
                timeRet->tm_hour,
                timeRet->tm_min,
                pdir->d_name);
        memcpy(msg.rst+offset,buf,strlen(buf));
        offset+=strlen(buf);
    }
    msg.rst[offset-1]='\0';
    msg.fileExit=1;
    memcpy(msg.cmd,"ls",2);
    send(netfd,&msg,sizeof(msg),0);
    closedir(dir);

}
int handlePWD(threadPool_t *threadPool,int netfd){
    rstMsg_t msg;
    bzero(&msg,sizeof(msg));
    memcpy(msg.cmd,"pwd",sizeof("pwd"));

    char absolutePath[2048]={0};
    user_t *user = getUserByFd(&threadPool->userArr,netfd);
    sprintf(absolutePath,"%s%s%s",threadPool->home,user->userName,user->userCwd);
    memcpy(msg.rst,absolutePath,strlen(absolutePath));
    send(netfd,&msg,sizeof(msg),0);
}
int handleEXIT(threadPool_t *threadPool,int netfd){
    rstMsg_t msg;
    bzero(&msg,sizeof(msg));
    memcpy(msg.cmd,"exit",sizeof("exit"));
    send(netfd,&msg,sizeof(msg),0);

    user_t *user = getUserByFd(&threadPool->userArr,netfd);
    printf("用户%s已下线！\n",user->userName);

    epollDel(threadPool->threadepfd,netfd);
    close(netfd);
    userArrDelByFd(&threadPool->userArr,netfd);
}
int handleCD(threadPool_t *threadPool,char *path,int netfd){
    // 获取用户的当前目录
    char *userName;
    user_t *user = getUserByFd(&threadPool->userArr,netfd);
    userName = user->userName;

    rstMsg_t msg;
    bzero(&msg,sizeof(msg));
    // 拼接出绝对路径与相对路径
    // 绝对路径：家目录+用户名+当前路径+想要切换的路径
    // 相对路径：当前路径+想要切换的路径
    char absolutePath[2048];
    char relativePath[2048];
    if(strcmp(path,".")==0){
        // 如果path是“.”则不进行任何操作。
        bzero(&msg,sizeof(msg));
        send(netfd,&msg,sizeof(msg),0);
        return 0;
    }
   // // 如果path是“..”则返回上一级
   // if(strcmp(path,"..")==0){
   //     if(user->dirStack.top<=0){
   //         // 如果当前路径为根目录，那么不能进入上一级
   //         bzero(&msg,sizeof(msg));
   //         msg.fileExit = 0;
   //         memcpy(msg.cmd,"cd",2);
   //         memcpy(msg.rst,"文件或目录不存在",strlen("文件或目录不存在"));
   //         send(netfd,&msg,sizeof(msg),0);
   //         return 0;
   //     }
   //     // 用户当前目录栈出队
   //     dirPop(&user->dirStack);
   //     getCwd(&user->dirStack,user->userCwd);
   //     //sprintf(absolutePath,"%s%s%s",threadPool->home,userName,user->userCwd);
   //     // 封装返回消息
   //     bzero(&msg,sizeof(msg));
   //     msg.fileExit = 1;
   //     memcpy(msg.cmd,"cd",2);
   //     memcpy(msg.userCwd,user->userCwd,strlen(user->userCwd));
   //     printf("userCwd:%s\n",user->userCwd);
   //     // 返回
   //     send(netfd,&msg,sizeof(msg),0);
   //     return 0;

   if(path[0]=='/'){
        //如果用户输入绝对路径
        sprintf(absolutePath,"%s%s%s",threadPool->home,userName,path);
        sprintf(relativePath,"%s",path);
    }else{
        sprintf(absolutePath,"%s%s%s%s",threadPool->home,userName,user->userCwd,path);
        sprintf(relativePath,"%s%s",user->userCwd,path);
    }
    printf("%s\n",absolutePath);
    printf("%s\n",relativePath);

    // 根据绝对路径查询是否存在
    int ret = access(absolutePath,F_OK);
    if(ret == 0){ // 想要切换的路径存在
        //printf("path: %s\n",path);
        // 将想要切换的路径压入用户路径栈中
        bzero(&msg,sizeof(msg));
        int rst = dirPushPath(&user->dirStack,path);
        if(rst == -1){
            // 说明入栈了../user1/dir
            msg.fileExit = 0;
            memcpy(msg.cmd,"cd",2);
            memcpy(msg.rst,"权限不足",strlen("权限不足"));
            send(netfd,&msg,sizeof(msg),0);
            return -1;
        }
        //printf("top=%d\n",user->dirStack.top);
        // 获得用户当前工作目录
        getCwd(&user->dirStack,user->userCwd);
        // 封装返回消息
        memcpy(msg.cmd,"cd",2);
        msg.fileExit = 1;
        memcpy(msg.userCwd,user->userCwd,strlen(user->userCwd));
        // 返回
        send(netfd,&msg,sizeof(msg),0);
    }else{
        msg.fileExit = 0;
        memcpy(msg.cmd,"cd",2);
        memcpy(msg.rst,"文件或目录不存在",strlen("文件或目录不存在"));
        send(netfd,&msg,sizeof(msg),0);
    }
}
