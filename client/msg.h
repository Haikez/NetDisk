#ifndef __MSG__
#define __MSG__
//typedef struct cmdMsg_s{
//    char *userName;
//    char *cmd;
//    char *path;
//
//}cmdMsg_t;
typedef struct rstMsg_s{
    // 用户当前工作目录
    char userCwd[1024];
    // 用户输入的命令
    char cmd[1024];
    // 打印在用户控制台的信息
    char rst[4096];
    // 判断文件是否存在
    int fileExit;
} rstMsg_t;
#endif
