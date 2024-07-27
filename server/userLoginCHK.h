#ifndef __USERLOGINCHK__
#define __USERLOGINCHK__
typedef struct userLoginMsg_s{
    char userName[1024];
    char userPasswd[1024];
}userLoginMsg_t;
//验证密码的正确性
int checkLoginMsg(userLoginMsg_t *loginMsg,int netfd);
#endif
