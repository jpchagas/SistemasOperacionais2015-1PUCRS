#include <stdio.h>
#include <pthread.h>

#define THREADS		3
#define DUMMY_LOOPS	10000000


int main(void){
	long int i;
	pthread_t threads[THREADS];
	
	sem_init(&mutex,0,1);

	for(i = 0; i < THREADS; i++)
		pthread_create(&threads[i], NULL, task, (void *)i);

	pthread_exit(NULL);	
	return(0);
}
