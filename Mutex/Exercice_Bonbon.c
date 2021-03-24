#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MAXBONBON 30
#define NB_BONBON_INITIALE 10
#define DELTAP 5000
#define DELTAC 3000
#define MIN_B 1
#define MAX_A 10
#define RAND(a,b) ((rand() % (b - a +1)) +a)

int bonbon = 0;
int deltac = DELTAC;
int deltap = DELTAP;

pthread_cond_t cond_c = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_p = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void disp_candy()
{
	printf("Vitrine : %3d [ ",bonbon);
	for (int i = 0; i < MAXBONBON; ++i)
	{
		if (i<bonbon)
		{
			printf("*");
		}
		else
		{
			printf("-");
		}
	}
	printf("]\n");
}

void * producteur()
{
	int nbb;
	while(1)
	{
		usleep(deltap + rand()%(deltap/10));
		nbb = RAND(MIN_B,MAX_A);
		pthread_mutex_lock(&mutex);
		while(bonbon + nbb > MAXBONBON)
		{
			pthread_cond_wait(&cond_p,&mutex);
		}
		bonbon += nbb;
		printf("producteur (%5hu)--> %02d : ",pthread_self(),nbb );
		disp_candy();
		pthread_cond_broadcast(&cond_c);
		pthread_mutex_unlock(&mutex);
	}
}

void * consommateur()
{
	int nbb;
	while(1)
	{
		usleep(deltac + rand()%(deltac/10));
	
		nbb = RAND(MIN_B,MAX_A);
		pthread_mutex_lock(&mutex);
		while(bonbon + nbb > MAXBONBON)
		{
			pthread_cond_wait(&cond_c,&mutex);
		}
		bonbon -= nbb;
		printf("consommateur (%5hu)--> %02d : ",pthread_self(),nbb );
		disp_candy();
		pthread_cond_broadcast(&cond_p);
		pthread_mutex_unlock(&mutex);
	}
}




int main(int argc, char const *argv[])
{
	int nb_producteur,nb_consommateur,qtte_bonbon;
	int deltap,deltac;

	if (argc < 5)
	{
		printf("Usage %s: <nb_producteur> <nb_consommateur> <qtte_bonbon> <delap> <deltac>\n",argv[0]);
		exit(-1);
	}
	srand(time(NULL));

	bonbon = strtol(argv[1],NULL,0);
	nb_consommateur = strtol(argv[3],NULL,0);
	nb_producteur = strtol(argv[2],NULL,0);

	deltap = strtol(argv[5],NULL,0);
	deltac = strtol(argv[6],NULL,0);

	pthread_t * prod = malloc(nb_producteur*sizeof(pthread_t));
	pthread_t * cons = malloc(nb_consommateur*sizeof(pthread_t));


	for (int i = 0; i < nb_producteur; ++i)
	{
		pthread_create(prod+i,NULL,producteur,NULL);
	}



	for (int i = 0; i < nb_consommateur; ++i)
	{
		pthread_create(cons+i,NULL,consommateur,NULL);
	}


for (int i = 0; i < nb_consommateur; ++i)
{
	pthread_join(cons[i],NULL);
}


for (int i = 0; i < nb_producteur; ++i)
{
	pthread_join(prod[i],NULL);
}



	return 0;
}