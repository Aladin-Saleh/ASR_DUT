#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include "ihm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <resolv.h>
#include <fcntl.h>
#include <sys/time.h>
#include <assert.h>

int main(int argc,char *argv[])
{
	fd_set main_fds,read_fds;
	int max_descriptor,socket_service;
	struct sockaddr_in client_addr= { 0 },server_addr = { 0 };
	int socket_ecoute; // socket d'ecoute
	ssize_t nbytes;
    int fd;
    char dst[INET_ADDRSTRLEN];
    char buf[1024];
	// preparation 
    socket_ecoute = socket(AF_INET,SOCK_STREAM,0);
    assert(socket_ecoute >=0);
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((in_port_t)strtol(argv[1],NULL,0));
    server_addr.sin_addr.s_addr = INADDR_ANY;
	// socket()
	// setsockopt() 
	// bind()
    bind(socket_ecoute,(struct sockaddr *)&server_addr,sizeof(server_addr));
	// listen()
    listen(socket_ecoute,15);

	FD_ZERO(&main_fds);
	FD_SET(socket_ecoute, &main_fds);
	max_descriptor = socket_ecoute ;
	while(1){
		read_fds = main_fds;
		if (select(max_descriptor+1,&read_fds,NULL,NULL,NULL)==-1){
			perror("select()");
			exit(1);
		}
		for (fd = 0;fd < max_descriptor; fd++){//on itere tous les descripteur potentielle
			if (FD_ISSET(fd,&read_fds)){
				if  (fd = socket_ecoute){ // new connection
                    socklen_t caddrlen = sizeof(client_addr);
					socket_service = accept(socket_ecoute,(struct sockaddr * )&server_addr,&caddrlen);
					if (socket_service >max_descriptor ) max_descriptor = socket_service ;
					FD_SET(socket_service, &main_fds); 
					inet_ntop(AF_INET,&client_addr.sin_addr,dst,INET_ADDRSTRLEN);
					printf("New connection from %s.%d\n",dst,ntohs(client_addr.sin_port));

				}else{ // read data from one client
					nbytes=read(fd,buf,sizeof(buf));
                    switch (nbytes)
                    {
                    case -1:
                        break;
                    case 0:
                    close(fd);
                    FD_CLR(fd,&main_fds);
                    if(fd == max_descriptor) max_descriptor--;
                    break;

                    default:
                    for (int k = 0; k < max_descriptor; k++)
                    {
                        if(FD_ISSET(k,&main_fds) && k != socket_ecoute && k!=fd)
                        {
                            write(k,buf,nbytes);
                        }
                    }

                    }
					
				}
			}	
		}
	}
}