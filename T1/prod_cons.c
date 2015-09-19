#include <stdio.h>
#include <pthread.h>

#define PROD	2
#define CONS	3

pthread_mutex_t m;

pthread_cond_t item_available;
pthread_cond_t space_available;

int items_in_buffer;
int max_items;

void init(void){
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&item_available, NULL);
	pthread_cond_init(&space_available, NULL);
	items_in_buffer = 0;
	max_items = 100;
}

void *consumer(void *arg){
	long int i;

	i = (long int)arg;
	for(;;){
		pthread_mutex_lock(&m);
		while (items_in_buffer == 0)
			pthread_cond_wait(&item_available, &m);
		items_in_buffer--;
		fprintf(stderr, "consumer %d, items %d\n", i, items_in_buffer);
		pthread_cond_signal(&space_available);
		pthread_mutex_unlock(&m);
	}
}

void *producer(void *arg){
	long int i;

	i = (long int)arg;
	for(;;){
		pthread_mutex_lock(&m);
		while (items_in_buffer == max_items)
			pthread_cond_wait(&space_available, &m);
		items_in_buffer++;
		fprintf(stderr, "producer %d, items %d\n", i, items_in_buffer);
		pthread_cond_signal(&item_available);
		pthread_mutex_unlock(&m);
	}
}

int main(void){
	long int i;
	pthread_t producers[PROD], consumers[CONS];

	init();

	for(i = 0; i < PROD; i++)
		pthread_create(&producers[i], NULL, producer, (void *)i);

	for(i = 0; i < CONS; i++)
		pthread_create(&consumers[i], NULL, consumer, (void *)i);

	pthread_exit(NULL);
	return(0);
}

