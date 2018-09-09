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

void showInfo(struct utmp *utbufp){
	printf("%-8.8s",utbufp->ut_name);
        printf(" ");
        printf("%-8.8s"utbufp->ut_line);
        printf(" ");
        printf("%10ld",utbufp->ut_time);
        printf(" ");
#ifdef SHOWHOST
        printf("(%s)",utbufp->ut_host);
#endif
	printf("\n");
}
