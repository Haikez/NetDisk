#include "tcpInit.h"
int tcpInit(const char *ipAddress, const char *port, int *psockfd){
    //socket setsockopt bind listen;
    *psockfd = socket(AF_INET,SOCK_STREAM,0);
    int reuse = 1;
    int ret = setsockopt(*psockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ERROR_CHECK(ret,-1,"setsockopt");
    
    // ip地址大小端转换
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipAddress);
    addr.sin_port = htons(atoi(port));

    //bind
    ret = bind(*psockfd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");

    //listen
    listen(*psockfd,50);
    return 0;
}
