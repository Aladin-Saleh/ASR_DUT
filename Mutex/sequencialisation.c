#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>

char *m[]={
	"0000","1111","2222","3333","4444","5555","6666","7777","8888","9999",
};

pthread_mutex_t lock;
pthread_cond_t cv;
int tour;
int tourMax = 5;
int nb_threads;

static inline double tstamp(void) 
{
	struct timespec tv;
	clock_gettime(CLOCK_REALTIME, &tv);
	return tv.tv_sec + tv.tv_nsec * 1.0e-9;
}


void * run (void *arg)
{
	int i,l,j,k;
	i = (long int)arg;
	l = strlen(m[i]);


	//printf("%d\n",nb_threads );

	for(k=0;k<100;k++){
		//<- code à ajouter
		pthread_mutex_lock(&lock);

		while(tour != i)
		{
			pthread_cond_wait(&cv,&lock);
		}

	for(j=0;j<l;j++){
			write(1,&m[i][j],1);
			usleep(100);
		}

		tour = ++tour%nb_threads;
	
	pthread_cond_broadcast(&cv);//Reveille tout les thread endormis


	pthread_mutex_unlock(&lock);


	}
}
int main(int argc, char * argv[])
{
	pthread_t * th;
	long int i;
	double t1,t2;
	if (argc !=2){
		nb_threads = 5;

	} else {
		nb_threads=((int)strtol(argv[1],NULL,0));
	}

	if (nb_threads<0) {
		nb_threads = 1;
	}

	if (nb_threads >10) {
		nb_threads = 10;

	}

	if (pthread_mutex_init(&lock, NULL) != 0) {
		printf("pthread_mutex_init failed\n");
		exit(-1);
	}

	th=(pthread_t *) malloc(nb_threads*sizeof(pthread_t));

	t1=tstamp();
	for(i=0;i<nb_threads;i++){
		pthread_create(th+i,NULL,run,(void*)i); // beurk :-)
	}

	for(i=0;i<nb_threads;i++){ 
		pthread_join(th[i],NULL);
	}
 	
	t2=tstamp();//TimeStamp
	printf("\n\ntime = %lf\n",t2-t1);

	pthread_mutex_destroy(&lock);
}