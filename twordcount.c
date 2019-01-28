#include "stdio.h"
#include "pthread.h"
#include "ctype.h"
struct arg_set{
	char *fname;
	int count;
};
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag=PTHREAD_COND_INITIALIZER;

struct arg_set *mailbox;
int total_words=0,reports_in=0;

int main(int ac,char **av){
	if(ac!=3){
		printf("usage!\n");
		return 1;
	}
	void *count_words(void *);
	struct arg_set arg1,arg2;
	pthread_t t1,t2;
//	pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
//	pthread_cond_t flag=PTHREAD_COND_INITIALIZER;

	pthread_mutex_lock(&lock);
	arg1.fname=av[1];
	arg1.count=0;
	pthread_create(&t1,NULL,count_words,(void *)&arg1);

	arg2.fname=av[2];
	arg2.count=0;
	pthread_create(&t2,NULL,count_words,(void *)&arg2);

	while(reports_in<2){
		printf("main:waiting\n");
		pthread_cond_wait(&flag,&lock);
		printf("flag raised!\n");
		printf("%s:%d\n",mailbox->fname,mailbox->count);
		total_words+=mailbox->count;
		if(mailbox==&arg1){
			pthread_join(t1,NULL);
		}
		if(mailbox==&arg2){
			pthread_join(t2,NULL);
		}
		mailbox=NULL;
		pthread_cond_signal(&flag);
		reports_in++;
//		pthread_mutex_unlock(&lock);
	}
	printf("total:%d\n",total_words);
	return 0;
}

void *count_words(void *a){
	struct arg_set *arg=a;
	int c,prevc='\0';
	FILE *fp;

	if((fp=fopen(arg->fname,"r"))!=NULL){
		while((c=getc(fp))!=EOF){
			if(!isalnum(c)&&isalnum(prevc)){
				arg->count++;
			}
			prevc=c;			
		}
		fclose(fp);
	}else{
		perror(arg->fname);
	}
	printf("%s waiting!\n",arg->fname);
	pthread_mutex_lock(&lock);
	printf("%s got a lock\n",arg->fname);
	while(mailbox!=NULL){
		pthread_cond_wait(&flag,&lock);
	}
	mailbox=arg;
	printf("%s raise flag\n",arg->fname);
	pthread_cond_signal(&flag);
	printf("%s unlocking\n",arg->fname);
	pthread_mutex_unlock(&lock);
	return NULL;
}
// prevc,
// arg_set,fname,count,mailbox,pthread_mutex_t,lock,pthread_cond_t,flag
// PTHREAD_COND_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,count_words,
// reports_in,total_words,pthread_mutex_lock,pthread_cond_wait,
// pthread_cond_signal,
