#include"Rio.h"

ssize_t rio_t::rio_read(char* usrbuf,size_t n){
    int cnt;
    while(rio_cnt<=0){
        rio_cnt =read(rio_fd,rio_buf,sizeof(rio_buf));
        if(rio_cnt<0){
            return -1;
        }
        else if(rio_cnt==0)
            return 0;
        else
            rio_bufptr = rio_buf;
    }
    cnt         = n;
    if(rio_cnt<n)
        cnt     = rio_cnt;
    memcpy(usrbuf,rio_bufptr,cnt);
    rio_bufptr += cnt;
    rio_cnt    -= cnt;
    return cnt;
}

ssize_t rio_t::rio_readlineb(char* usrbuf,size_t maxlen){
    int  n,rc;
    char c,*bufptr =(char*)usrbuf;

    for(n=0;n!=maxlen;n++){
        if((rc=rio_read(&c,1))  == 1){
            *bufptr          ++= c;
            if(c=='\n')
                break;
        }
        else if(rc==0){
            if(n==0)
                return 0;
            else
                break;
        } else
            return -1;
    }
    *bufptr = 0;
    return n;
}
ssize_t rio_writen(int fd, void *usrbuf, size_t n) 
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = (char *)usrbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR)  /* interrupted by sig handler return */
                nwritten = 0;    /* and call write() again */
            else
                return -1;       /* errno set by write() */
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}
