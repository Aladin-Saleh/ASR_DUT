#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>

int sock_bind(struct addrinfo * res,struct addrinfo *s);
int print_where_bound(struct addrinfo s);

char host[NI_MAXHOST], service[NI_MAXSERV],buf[1024];

int main(int argc,char *argv[])
{
	fd_set fds,fdsc;
	int nfds,newfd,k,nbbytes,j;
	struct addrinfo caddr,*saddr,s;
	int se; // socket d'ecoute
	ssize_t nbytes;

	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s <port_numb>\n",argv[0]);
		exit(1);
	}


	memset(&caddr,0,sizeof(struct addrinfo));

	caddr.ai_family=AF_UNSPEC;
	caddr.ai_socktype=SOCK_STREAM;//tcp
	caddr.ai_protocol=0;
	caddr.ai_flags=AI_NUMERICSERV | AI_PASSIVE;
	caddr.ai_canonname=NULL;
	caddr.ai_addr=NULL;
	caddr.ai_next=NULL;

	getaddrinfo(NULL,argv[1],&caddr,&saddr);
	
	se = sock_bind(saddr,&s);

	freeaddrinfo(saddr);
	print_where_bound(s);	

	if ((listen(se, 10)) != 0) 
	{ 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
    {
        printf("Server listening..\n"); 
    }
  	
	FD_ZERO(&fds);
	FD_SET(se, &fds);
	nfds = se;
	while(1){
		fdsc = fds;
		if (select(nfds+1,&fdsc,NULL,NULL,NULL)==-1)
		{
			perror("select()");
			exit(1);
		}
		for (j=0 ; j <= nfds; j++)
		{
			if (FD_ISSET(j,&fdsc)){
				if  (j==se){ // new connection
					newfd = accept(se, NULL, NULL);
					if (newfd  > nfds) nfds = newfd;
					FD_SET(newfd , &fds); 
					printf("accept %d\n",nfds);
					//inet_ntop(AF_INET,&caddr.sin_addr,dst,INET_ADDRSTRLEN);
					/*printf("New connection from %s.%d\n",
							dst,ntohs(caddr.sin_port));*/

				}else{ // read data from one client
					nbbytes =read(j,buf,sizeof(buf));
					//printf("\n");
					switch(nbbytes)
   					{
   						case -1 : 
   						{ 
   							perror("read()"); exit(1);
   						}
   						case 0 : 
   						{	
   							printf("Client on socket %d has left\n",newfd); 
   							close(newfd); 
   							nfds-=1;
   						}
   						default : 
   						{
   							printf("reception\n");
							for (k = 0 ; k <=  nfds; k++) 
							{
								if (FD_ISSET(k, &fds ) && k != se  && k != j )  
								{
									write(k,buf,nbbytes); // may write block ? yes :-(
								}
							}
   						}
   					}

					/*if (nbbytes == -1) {
						perror("read()");
						exit(1);
					}
					if (nbbytes == 0){
						printf("Client on socket %d has left\n",newfd);
						FD_CLR(newfd,&fds);
						close(newfd);

						nfds-=1;

					}
					else{ // Data are sent to all the clients
						printf("reception\n");
						for (k = 0 ; k <=  nfds; k++) 
						{
							if (FD_ISSET(k, &fds ) && k != se  && k != j )  
							{
								write(k,buf,nbbytes); // may write block ? yes :-(
							}
						}
					}*/
				}
			}	
		}
	}
}

int print_where_bound(struct addrinfo s)
{
	int r = getnameinfo(s.ai_addr, s.ai_addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST);
	printf("%s.%s\n", host, service);
	printf("fam: %d -- socktype: %d -- pro: %d -- addrlen: %d -- canonname: %s\n",s.ai_family, s.ai_socktype, s.ai_protocol, s.ai_addrlen, s.ai_canonname);
	return r;
}

int sock_bind(struct addrinfo * res,struct addrinfo *s)
{
	int r,sfd = -1;
	while(res!=NULL)
	{
		sfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);		
		if(sfd>0)
		{
			r=bind(sfd,res->ai_addr,res->ai_addrlen);
			if(r==0)
			{
				*s=*res;
				break;
			}
			else
			{
				close(sfd);
				sfd=-1;				
			}					
		}
		res=res->ai_next;
	}
	return sfd;
}