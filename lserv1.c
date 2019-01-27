#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "signal.h"
#include "sys/errno.h"
#include "dgram.c"

#define MSGLEN 128
#define SERVER_PORTNUM 2020
#define TICKET_AVAIL 0
#define MAXUSERS 3
#defien oops(x) {perror(x);exit(-1);}
int main(){
	struct sockaddr client_addr;
	socklen_t addrlen=sizeof(client_addr);
	int ret,sock;
	char buf[MSGLEN];
	sock=setup();

	while(1){
		ret=recvfrom(sock,buf,MSGLEN,0,&client_addr,&addrlen);
		if(ret>0){
			buf[ret]='\0';
			narrate("GOT:",buf,&client_addr);
			handle_request(buf,&client_addr,addrlen);
		}else if(errno!=EINTR){
			perror("recvfrom");
		}
	}
	return 0;
}
int sd=-1;
int ticket_array[MAXUSERS];
int num_tickets_out=0;

int setup(){
	sd=make_dgram_client_socket(SERVER_PORTNUM);
	if(sd==-1){
		oops("cannot get socket");
	}

	free_all_tickets();
	return sd;
}

void free_all_tickets(){
	for(int i=0;i<MAXUSERS;i++){
		ticket_array[i]=TICKET_AVAIL;
	}
}

void shut_down(){
	close(sd);
}

void handle_request(char *rq,struct sockaddr *caddr,socklen_t addrlen){
	char *response;
	if(strncmp(rq,"HELO",4)==0){
		response=do_hello(rq);
	}else if(strncmp(rq,"GBYE",4)==0){
		response=do_goodbye(rq);
	}else{
		response="FAIL invalid request";
	}
	narrate("SAID:",rq,caddr);
	
	if(sendto(sd,response,strlen(response),0,caddr,addrlen)==-1){
		perror("sendto");
	}
	
}

char *do_hello(char *msg){
	static char replybuf[MSGLEN];
	int i;

	if(num_tickets_out>=MAXUSERS){
		sprintf(replybuf,"FAIL no ticket left");
	}else{
		for(i=0;i<MAXUSERS&&(ticket_array[i]!=TICKET_AVAIL);i++);
		ticket_array[i]=atoi(msg+5);
		sprintf(replybuf,"TICK %d.%d",ticket_array[i],i);
		num_tickets_out++;
	}
	return replybuf;
}

char *do_goodbye(char *msg){
	int pid,slot;
	if(sscanf(msg+5,"%d.%d",&pid,&slot)!=2){
		narrate("Bogus ticket",msg+5,NULL);
		return "FAIL Bogus ticket";
	}

	if(ticket_array[slot]!=pid){
		narrate("bogus ticket",msg+5,NULL);
		return "FAIL bogus ticket";
	}
	ticket_array[slot]=TICKET_AVAIL;
	num_tickets_out--;
	return "THNX goodbye";
}
void narrate(char *m1,char *m2,struct sockaddr *clientp){
	fprintf(stderr,"\t\tSERVER:%s %s\n",m1,m2);
	if(clientp){
		fprintf(stderr,"Client(%s:%d)\n",inet_ntoa(clientp->sin_addr,),ntohs(clientp->sin_port));
	}
}
// inet_ntoa,ntohs
// slot,Bogus
// replybuf,
// ticket_array,num_tickets_out,free_all_tickets,
// socklen_t,client_addr,addrlen,narrate,handle_request,errno,EINTR
