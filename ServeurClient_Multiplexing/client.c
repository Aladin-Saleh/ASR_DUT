#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <fcntl.h>
#include <sys/time.h>


extern ihmNcu ihm; // l'ihm
int sid=-1;         // la socket pour la connexion au serveur
char * user=NULL;   // le nom de l'utilisateur

void sigint_handler(int i){

	// envoye un message de deconnection
	// au serveur
	ihmEnd();
	exit(0);
}

int main(int argc, char * argv[])
{
	char buf[256];
	struct sigaction sa;
	fd_set fdmain,fdread;
	struct sockaddr_in saddr;
	int addrlen; 

	if (argc != 4) {
		printf("Usage: %s <IP-address> <port number> <user>\n",argv[0]);
		exit(1);
	}
	user=argv[3];


	ihmStart();
	infoLine("127.0.0.1", argv[2]);
	wprintTime(ihm.tchatWin, time(NULL));
	wprintw(ihm.tchatWin, "%s\n", "Bonjour!");

	while(1) {
		ihmRefresh();
		readTextBox(user,buf,sizeof(buf));
	}
}

