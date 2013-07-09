#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<string.h>
#include "httpsession.h"

#define CONFIG config
#define BACKLOG 20
using namespace std;

typedef struct sockaddr    SA;
typedef struct sockaddr_in SAL;

int Init(int& );
int Open_listen(int );
int Accept(int,SAL&);

int main(){
    int port;
    SAL serveraddr,clientaddr;
    int listenfd,connfd;
    pid_t pid;
        
    
    Init(port);
    listenfd = Open_listen(port);

    while((connfd=Accept(listenfd,clientaddr)) != -1){
        pid                          = fork();
        if(pid==0){
            pid                      = fork();
            if(pid==0)
                break;
            else
                exit(0);//连续两次fork() 避免僵死进程
        }else continue;
    }

    if(pid==0){
        HttpSession session(connfd);
        session.Handler();
    }
       
    return 0;
    }


int Init(int& port){
    FILE* fp ;
    char  buf[10];
    if((fp=fopen("config","r")) == NULL){
        fprintf(stderr,"fopen error\n");
        ;                       //错误处理，初始化失败TODO
        exit(0);
    }
    fgets(buf,6,fp);
    if(strcmp(buf,"port:") == 0)
         fscanf(fp,"%d",&port);
     else
             ;                       //错误处理，初始化失败TODO
    return 0;
}

int Open_listen(int port){
    int                listenfd,optval = 1;
    struct sockaddr_in serveraddr;

    if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0)
        return -1;

    memset((char*)&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family      = AF_INET;
      serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port        = htons((unsigned short)port);

    if(bind(listenfd,(SA*)&serveraddr,sizeof(serveraddr))!=0)
        return -1;//错误处理 TODO

    if(listen(listenfd,BACKLOG)!=0)
        return -1;// 错误处理 TODO
    return listenfd;
}

int Accept(int listenfd,SAL& clientaddr){
    unsigned clientlen = sizeof(clientaddr);
    int      connfd    = accept(listenfd,(SA*)&clientaddr,&clientlen);
    return connfd;
}
