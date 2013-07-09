/*
 * tinyweb.cc
 *
 *  Created on: Dec 11, 2012
 *  
 *  主程序流程
 *      1.打开系统日志,设置成静态
 *      2.初始化记时器，记录系统运行时间
 *      3.初始化服务器：建立套接字，设置监听...
 *      4.主循环
 *      {
 *              fork()...
 *      }
 *      5.关闭http server对象
 *      6.关闭系统日志
 *      7.退出
 *包括在这里定义初始化函数Init()
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<string.h>
#include "config.h"
#include "httpsession.h"
#include "syslog.h"

#define PORT 8080
#define BACKLOG 20//这个是最大连接数量？

typedef struct sockaddr SA;
typedef struct sockaddr_in SAL;


int     Init(SAL& );            //初始化服务器
int     Accept();               //建立连接
int main(int argc, char **argv) {
        //TODO:先不管日志文件
    SAL serveraddr;
    int listenfd;
    
    listenfd = Init(serveraddr);
}

//TODO:Init()实现
int Init(SAL& serveraddr) {
    memset((char*)serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port        = htol((unsigned)PORT);

    if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0)
       ;                        //错误处理 TODO
    
    if(bind(listenfd,(SA*)serveraddr,sizeof(serveraddr))<0)
        ;                       //错误处理
    
    if(listen(listenfd,LISTENQ)<0)
        ;                       //错误处理
    return listenfd;
}
//TODO：Accept()实现
int Accept(SAL& clientaddr,int listenfd) {
    unsigned clientfd = sizeof(clientfd);
    int      connfd   = accept(listenfd,(SA*)clientaddr,&clentfd);
    return connfd;
    
}
