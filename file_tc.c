#include "stdio.h"
#include "sys/file.h"
#include "fcntl.h"
#include "stdlib.h"
#include "unistd.h"

#define oops(m,x) {perror(m);exit(x);}
#define BUFLEN 20

void lock_operation(int fd,int op){
	struct flock lock;
	lock.l_whence=SEEK_SET;
	lock.l_start=lock.l_len=0;
	lock.l_pid=getpid();
	lock.l_type=op;
	if(fcntl(fd,F_SETLKW,&lock)==-1)oops("fcntl",1);
}

int main(int ac,char **av){
	int fd,nread;
	char buf[BUFLEN];

	fd=open(av[1],O_RDONLY);
	if(fd==-1)oops("open",1);
	lock_operation(fd,F_RDLCK);
	nread=read(fd,buf,BUFLEN);
	if(nread==-1)oops("read",1);
	lock_operation(fd,F_UNLCK);
	write(1,buf,nread);
	close(fd);
}
// O_RDONLY,F_RDLCK,F_UNLCK,l_whence,l_start,l_len,pid,type,F_SETLKW
