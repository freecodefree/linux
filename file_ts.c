#include "stdio.h"
#include "sys/file.h"
#include "fcntl.h"
#include "time.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

#define oops(m,x) {perror(m);exit(x);}

int main(int ac,char **av){
	time_t now;
	char *message;
	int fd;
	
	fd=open(av[1],O_CREAT|O_TRUNC|O_WRONLY,0644);
	if(fd==-1)oops("open",1);

	void lock_operation(int,int);
	while(1){
		time(&now);
		message=ctime(&now);
		lock_operation(fd,F_WRLCK);
		if(lseek(fd,0L,SEEK_SET)==-1)oops("lseek",1);
		if(write(fd,message,strlen(message))==-1)oops("write",1);
		lock_operation(fd,F_UNLCK);
		sleep(1);
	}
}

void lock_operation(int fd,int op){
	struct flock lock;
	lock.l_whence=SEEK_SET;
	lock.l_start=lock.l_len=0;
	lock.l_pid=getpid();
	lock.l_type=op;

	if(fcntl(fd,F_SETLKW,&lock)==-1)oops("fcntl",1);
}
// flock,l_whence,l_start,l_pid,getpid,l_type,l_len,fcntl,F_SETLKW,
// time_t,message,O_CREAT,O_TRUNC,O_WRONLY,0644,lock_operation,F_WRLCK,
// SEEK_SET,
