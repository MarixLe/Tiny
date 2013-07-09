#ifndef RIO__H_
#define RIO__H_

#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

#define RIO_BUFSIZE 8192;

class rio_t{
public:

    rio_t(int fd){
        rio_fd     = fd;
        rio_cnt    = 0;
        rio_bufptr = rio_buf;
    }
    ssize_t rio_read(char* usrbuf,size_t n);
    ssize_t rio_readlineb(char* usrbuf,size_t maxlen);
            
private:
    char  rio_buf[8192];
    int   rio_fd;
    int   rio_cnt;
    char* rio_bufptr;
};
ssize_t rio_writen(int , void *, size_t );
#endif
