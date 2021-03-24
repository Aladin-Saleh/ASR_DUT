
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <fcntl.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ihm.h"
#include<time.h>

#include <sys/types.h>
#include <signal.h>
#include <netdb.h>

int try_socket(struct addrinfo *res,struct addrinfo *s);
int print_where_bound(struct addrinfo s);
char host[NI_MAXHOST], service[NI_MAXSERV],buf[1024];

extern ihmNcu ihm; // l'ihm
int sid=-1;         // la socket pour la connexion au serveur
char * user=NULL;   // le nom de l'utilisateur

void sigint_handler(int i){

	// envoye un message de deconnection
	// au serveur
	close(sid);
	ihmEnd();
	exit(0);
}

int main(int argc, char * argv[])
{
	char buf[256];
	struct sigaction sa;
	fd_set fdmain,fdread;
	struct addrinfo criteria;
	struct addrinfo s,*res;
	int addrlen; 

	if (argc != 4) {
		printf("Usage: %s <IP-address> <port number> <user>\n",argv[0]);
		exit(1);
	}
	user=argv[3];

	memset(&criteria,0,sizeof(struct addrinfo));

	criteria.ai_family=AF_UNSPEC;
	criteria.ai_socktype=SOCK_STREAM;
	criteria.ai_protocol=0;
	criteria.ai_flags=AI_NUMERICSERV;
	criteria.ai_canonname=NULL;
	criteria.ai_addr=NULL;
	criteria.ai_next=NULL;

	getaddrinfo(argv[1],argv[2],&criteria,&res);
	sid = try_socket(res,&s);
	print_where_bound(s);
	if(connect(sid,res->ai_addr,res->ai_addrlen) != 0)
	{
		printf("Echec de la connection au serveur\n");
		exit(0);
	}
	printf("Connection reussi\n");

	ihmStart();
	infoLine("127.0.0.1", argv[2]);
	wprintTime(ihm.tchatWin, time(NULL));
	wprintw(ihm.tchatWin, "%s\n", "Bonjour!");

	while(1) {
		ihmRefresh();
		readTextBox(user,buf,sizeof(buf));
		sendto(sid,(const char *)buf,strlen(buf),0,res->ai_addr,res->ai_addrlen);
		//recvfrom(sid, buf, sizeof(buf), 0,res->ai_addr,res->ai_addrlen);
		
	}
	close(sid);
}

int print_where_bound(struct addrinfo s)
{
	int r = getnameinfo(s.ai_addr, s.ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST);
	printf("%s.%s\n", host, service);
	printf("fam: %d -- socktype: %d -- pro: %d -- addrlen: %d -- canonname: %s\n",s.ai_family, s.ai_socktype, s.ai_protocol, s.ai_addrlen, s.ai_canonname);
	return r;
}

int try_socket(struct addrinfo *res,struct addrinfo *s)
{
	int sfd=-1;
	while(res!=NULL)
	{
		sfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if(sfd>0)
		{
			*s=*res;
			break;			
		}
		res=res->ai_next;
	}
	return sfd;
}




