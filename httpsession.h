
#ifndef HTTPSESSION_H_
#define HTTPSESSION_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include<sys/mman.h>
#include<sys/types.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <netinet/in.h>

#include"Rio.h"


#define RECV_BUFFER_SIZE        1024
#define SEND_BUFFER_SIZE        1050000
#define URI_SIZE                128

#define TIME_OUT_SEC            10      //超时时间
#define TIME_OUT_USEC           0

#define FILE_OK                 200     //成功
#define FILE_REMOVE             301     //永久移动
#define REQUEST_ERR             400     //请求错误，服务器不能理解请求
#define FILE_FORBIDEN           403     //无权限
#define FILE_NOT_FOUND          404     //未找到
#define UNSUPPORT_MIME_TYPE     415     //不支持的MIME类型
#define UNSUPPROT_METHOD        501     //不支持方法
#define UNSUPPORT_HTTP_VERSION  505     //服务器不支持HTTP版本
#define FILE_MAX_SIZE           1048576         /* 1MB the max siee of file read from hard disk */

#define SERVER                  "Server:tiny_webserver(0.1 Alpha)/Ubuntu"

class HttpSession {
public:
    HttpSession(int );
    virtual ~HttpSession();
    void   Handler();            //处理一次请求的过程
private:
    rio_t rio;
    int fd;

    int  parse_uri(char* ,char* ,char* );
    void clienterror(int,char* ,char* ,char* ,char*);
    void server_static(int ,char* ,int );
    void get_filetype(char* ,char* );
    void read_requestdrs();
        
};


#endif
