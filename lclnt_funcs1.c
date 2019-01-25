#include "stdio.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "netdb.h"
#include "dgram.c"

#define MSGLEN 128
#define SERVER_PORTNUM 2020
#define HOSTLEN 512
#define oops(p) {perror(p);exit(1);}

static int pid=-1;
static int sd=-1;
static struct sockaddr serv_addr;
static struct socklen_t serv_alen;
static char ticket_buf[128];
static int have_ticket=0;

char *do_transaction();

void setup(){
	char hostname[BUFSIZ];
	pid=getpid();

	sd=make_dgram_client_socket();
	if(sd==-1){
		oops("cannot got a socket");
	}

	gethostname(hostname,HOSTLEN);

	make_internet_address(hostname,SERVER_PORTNUM,&serv_addr);
	serv_alen=sizeof(serv_addr);
}

void shut_down(){
	close(sd);
}

int get_ticket(){
	char *response;
	char buf[MSGLEN];

	if(have_ticket){
		return 0;
	}
	
	sprintf(buf,"HELO %d",pid);
	if((response=do_transaction(buf))==NULL){
		return -1;
	}

	if(strncmp(response,"TICK",4)==0){
		have_ticket=1;
		strcpy(ticket_buf,response+5);
		narrate("got a ticket",ticket_buf);
		return 0;
	}

	if(strncmp(response,"FAIL",4)==0){
		narrate("cannot get a ticket",response);
	}else{
		narrate("unknown message:",response);
	}
	
	return -1;	
}

int release_ticket(){
	char buf[MSGLEN];
	char *response;

	if(!have_ticket)return 0;

	sprintf(buf,"GBYE %s",ticket_buf);
	if((response=do_transaction(buf))==NULL){
		return -1;
	}

	if(strncmp(response,"THNX",4)==0){
		have_ticket=0;
		narrate("successful","");
		return 0;		
	}

	if(strncmp(response,"FAIL",4)==0){
		narrate("faill",response+5);
	}else{
		narrate("unknown message:",response);
	}
	return -1;
}

char *do_transaction(char *msg){
	static char buf[MSGLEN];
	struct sockaddr retaddr;
	socklen_t addrlen=sizeof(retaddr);
	int ret;

	ret=sendto(sd,msg,strlen(msg),0,&serv_addr,serv_alen);
	if(ret==-1){
		syserr("sendto");
		return NULL;
	}

	ret=recvfrom(sd,buf,MSGLEN,0,&retaddr,&addrlen);
	if(ret==-1){
		syserr("recvfrom");
		return NULL;
	}
	return buf;
}

void narrate(char *msg1,char *msg2){
	fprintf(stderr,"CLIENT [%d]:%s%s\n",pid,msg1,msg2);
}

void syserr(char *msg){
	char buf[MSGLEN];
	sprintf(buf,"CLINET [%d]:%s\n",pid,msg);
	perror(buf);
}

void do_regular_work(){
	printf("Sleep running\n");
	sleep(10);
}

int main(int ac,char **av){
	setup();
	if(get_ticket()!=0){
		printf("no ticket\n");
		return 1;
	}
	do_regular_work();
	release_ticket();
	shut_down();
	return 0;
}
// retaddr,socklen_t,addrlen,ret,serv_addr,serv_alen,syserr
// shut_down,get_ticket,response,buf,do_transaction,narrate, 
