#ifndef __LOGIN__
#define __LOGIN__
typedef struct userLoginMsg_s{
    char userName[1024];
    char userPasswd[1024];
}userLoginMsg_t;
void sendLoginMsg(int sockft);
#endif
