#include <myfunc.h>
#include <errno.h>
#include "dirStack.h"
int main()
{
    int rst = unlink("dir1");
    if(rst==-1){
        perror("unlink:");
    }
    //int rst = mkdir("./dir1",0775);
    //char buf[1024]={0};
    //getcwd(buf,1024);
    //printf("cwd: %s\n",buf);
    //if(rst==-1){
    //    printf("创建失败:\n");
    //    perror("mkdir");
    //}
    //int lastSize=0;
    //char buf[1024]={0};
    //char tmp[1024]={0};
    //int a = 4;
    //char *str1="hello";
    //int p=strlen(str1);
    //sprintf(buf,"%s %d\n",str1,a);
    //memcpy(tmp,buf,strlen(buf));
    //lastSize+=strlen(buf);

    //char *str2="world\n";
    //bzero(buf,sizeof(buf));
    //sprintf(buf,"%s",str2);
    //memcpy(tmp+lastSize,buf,strlen(buf));

    //printf("%s",tmp);
    //printf("bufSize: %ld",strlen(buf));
    //DIR *dir = opendir("test.o");
    //if(!dir){
    //    char *str = strerror(errno);
    //    printf("%s\n",str);
    //    return 0;
    //}
    //struct dirent *p; 
    //while((p = readdir(dir))!=NULL){
    //    printf("%s\n",p->d_name);
    //}
//    int netfd = open("test.o",O_RDONLY);
//    printf("%d\n",netfd);
    //close(netfd);
    //printf("%d\n",netfd);


  //  char str[]="hi";
  //  char *each,*pstr;
  //  pstr=str;
  //  dirStack_t dirStack;
  //  dirStackInit(&dirStack);
  //  char buf[1024]={0};
  //  if(str[0]=='/'){ // 如果输入的是绝对路径
  //                   // 如果输入的是绝对路径 
  //                   // 清空栈                                         
  //      bzero(dirStack.dirName,sizeof(dirStack.dirName));
  //      dirStack.top=-1;
  //      dirPush(&dirStack,"/"); 
  //  }
  //  while((each = strtok(pstr,"/"))!=NULL){
  //      sprintf(buf,"%s/",each);
  //      dirPush(&dirStack,buf);
  //      bzero(buf,sizeof(buf));
  //      pstr=NULL;
  //  }

  //  char *path;
  //  path = getCwd(&dirStack);
  //  printf("%s\n",path);
  

    //printf("%ld\n",strlen(path));
    //printf("%d\n",dirStack.top);



    //int rst = dirPush(&dirStack,str);
    //if(rst){
    //    printf("%s\n",dirStack.dirName[0]);
    //}

    //char str[1024];
    //memcpy(str,"hello",sizeof("hello"));
    //printf("strlen:%ld\n",strlen(str));

    //int ret = access("dir2/",F_OK);
    //ERROR_CHECK(ret,-1,"access");
    //if(ret == 0){
    //    printf("存在\n");
    //}else{
    //    printf("不存在\n");
    //}
    return 0;
}

