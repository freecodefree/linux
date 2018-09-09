#include "stdio.h"
#include "utmp.h"
#include "fcntl.h"
#include "unistd.h"

#define SHOWHOST

int main(){
	struct utmp currentRecord;
	int utmpFd;
	int reclen=sizeof(currentRecord);

	if((utmpFd=open(UTMP_FILE,O_RDONLY))==-1){
		perror(UTMP_FILE);
		exit(1);
	}

	while(read(utmpFd,&currentRecord,reclen)==reclen){
		show_info(&currentRecord);
	}

	close(utmpFd);
	return 0;
}
