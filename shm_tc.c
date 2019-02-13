#include "stdio.h"
#include "sys/shm.h"
#include "time.h"
#include "stdlib.h"

#define TIME_MEM_KEY 99
#define SEG_SIZE ((size_t)100)
#define oops(m,x){perror(m);exit(x);}

int main(){
	int seg_id;
	char *mem_ptr;

	seg_id=shmget(TIME_MEM_KEY,SEG_SIZE,0777);
	if(seg_id==-1)oops("shmget",1);

	mem_ptr=shmat(seg_id,NULL,0);
	if(mem_ptr==(void *)-1)oops("shmat",1);

	printf("from server:%s\n",mem_ptr);

	shmdt(mem_ptr);
}
// seg_id,mem_ptr,shmget,0777,shmat,shmdt
