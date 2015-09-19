#include <stdio.h>
#include <pthread.h>

#define READERS	3
#define WRITERS	3

pthread_mutex_t m;
pthread_cond_t turn;
int reading;
int writing;
int writers;

void init(void){
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&turn, NULL);
	reading = 0;
	writing = 0;
	writers = 0;
}


void *reader(void *arg){
	long int i;

	i = (long int)arg;
	for(;;){
		pthread_mutex_lock(&m);
		if (writers)
			pthread_cond_wait(&turn, &m);
		while(writing)
			pthread_cond_wait(&turn, &m);
		reading++;
		pthread_mutex_unlock(&m);
		fprintf(stderr, "reader %d (of %d)\n", i, reading);
		pthread_mutex_lock(&m);
		reading--;
		pthread_cond_broadcast(&turn);
		pthread_mutex_unlock(&m);
	}
}

void *writer(void *arg){
	long int i;

	i = (long int)arg;
	for(;;){
		pthread_mutex_lock(&m);
		writers++;
		while(reading || writing)
			pthread_cond_wait(&turn, &m);
		writing++;
		pthread_mutex_unlock(&m);
		fprintf(stderr, "writer %d (of %d)\n", i, writing);
		pthread_mutex_lock(&m);
		writing--;
		writers--;
		pthread_cond_broadcast(&turn);
		pthread_mutex_unlock(&m);
	}
}

int main(void){
	long int i;
	pthread_t readers[READERS], writers[WRITERS];

	init();

	for(i = 0; i < READERS; i++)
		pthread_create(&readers[i], NULL, reader, (void *)i);

	for(i = 0; i < WRITERS; i++)
		pthread_create(&writers[i], NULL, writer, (void *)i);

	pthread_exit(NULL);
	return(0);
}

