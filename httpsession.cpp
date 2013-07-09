
#include "httpsession.h"

#define MAXLINE 1024
#define MAXBUF  8192
HttpSession::HttpSession(int connfd):rio(connfd) {
     fd=connfd;
//     rio(fd);
}

void HttpSession::Handler(){

    int         is_static;
    struct stat sbuf;
    char        buf[MAXLINE];
    char        method[MAXLINE],uri[MAXLINE],version[MAXLINE];
    char        filename[MAXLINE], cgiargs[MAXLINE];
    //以上都可作为类私有成员，但觉得作用不大
        
    rio.rio_readlineb(buf,MAXLINE);
    sscanf(buf,"%s %s %s",method,uri,version);

    if(strcasecmp(method,"GET")){
        clienterror(fd,method,"501","Not Implement",
                    "Server doesn't implement this method");
        read_requestdrs();
    }
    is_static = parse_uri(uri,filename,cgiargs);
    if(stat(filename,&sbuf)<0){
        clienterror(fd,filename,"404","Not found",
                   "Server couldn't find this file");
        return;
    }

    if(is_static){
        if(!(S_ISREG(sbuf.st_mode))|| (!S_IRUSR & sbuf.st_mode)){
            clienterror(fd,filename,"403","Forbidden",
                        "server couldn't read the filem");
            return;
        }
        server_static(fd,filename,sbuf.st_size);
    }
    else{
        if(!(S_ISREG(sbuf.st_mode))||(!S_IRUSR& sbuf.st_mode)){
            clienterror(fd,filename,"403","Forbidden",
                        "Server couldn't run the program");
            return;
        }
        //server_dynamic();  //调用动态内容
    }

}


int HttpSession::parse_uri(char* uri,char* filename,char* cgiargs){
    char* ptr;

    if(!strstr(uri,"cgi-bin")){
        strcpy(cgiargs,"");
        strcpy(filename,".");
        strcat(filename,uri);
        if(uri[strlen(uri)-1] == '/')
            strcat(filename,"home.html");
        return 1;
    }
    else{
        ptr = index(uri,'?');
        if(ptr){
            strcpy(cgiargs,ptr+1);
            *ptr = '\0';
        }
        else
            strcpy(cgiargs,"");
        
        strcpy(filename,".");
        strcat(filename,uri);
        return 0;
    }
}

void HttpSession::read_requestdrs(){
    char buf[MAXLINE];
    rio.rio_readlineb(buf,MAXLINE);
    while(strcmp(buf,"\r\n"))
        rio.rio_readlineb(buf,MAXLINE);
    return ;
}
void HttpSession::clienterror(int   fd,char* cause,char* errnum,
                 char* shortmsg,char* longmsg){
    char               buf[MAXLINE],body[MAXBUF];

    sprintf(body,"<html><title>Tiny Error</title>");
    sprintf(body,"%s<body bgcolor=""ffffff"">\r\n",body);
    sprintf(body,"%s%s: %s\r\n",body,errnum,shortmsg);
    sprintf(body,"%s<p>%s: %s\r\n",body,longmsg,cause);
    sprintf(body,"%s<hr><em>The Tiny Web server></em>\r\n",body);

    sprintf(buf,"HTTP/1.0 %s\r\n",errnum,shortmsg);
    rio_writen(fd,buf,strlen(buf));
    sprintf(buf,"Content-type: text/html\r\n");
    rio_writen(fd,buf,strlen(buf));
    sprintf(buf,"Content-length: %d\r\n\r\n",strlen(body));
    rio_writen(fd,buf,strlen(buf));
    rio_writen(fd,body,strlen(body));
}
///错误处理和输出函数，根据你定义的宏修改吧

void HttpSession::get_filetype(char* filename,char* filetype){
    if(strstr(filename,".html"))
        strcpy(filetype,"text/html");
    else if(strstr(filename,".gif"))
        strcpy(filetype,"image/gif");
    else if(strstr(filename,".jpg"))
        strcpy(filetype,"image/jpeg");
    else if(strstr(filename,"mp4"))
        strcpy(filetype,"vedio/mpg");
    else strcpy(filetype,"text/plain");
}


void HttpSession::server_static(int fd,char* filename,int filesize){
    int srcfd;

    char* srcp,filetype[MAXLINE],buf[MAXBUF];

    get_filetype(filename,filetype);
    sprintf(buf,"HTTP/1.0 200 OK\r\n");
    sprintf(buf,"%sServer:Tiny Web Server\r\n",buf);
    sprintf(buf,"%sContent-length: %d\r\n",buf,filesize);
    sprintf(buf,"%sContent_type: %s\r\n\r\n",buf,filetype);
    rio_writen(fd,buf,strlen(buf));

    srcfd = open(filename,O_RDONLY,0);
    srcp  =(char*) mmap(0,filesize,PROT_READ,MAP_PRIVATE,srcfd,0);
    close(srcfd);
    rio_writen(fd,srcp,filesize);
    munmap(srcp,filesize);
}
// 输出静态网页



HttpSession::~HttpSession() {

}
