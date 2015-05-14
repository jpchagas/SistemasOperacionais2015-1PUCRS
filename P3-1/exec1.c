#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t S1, S2, S3, S4;
int i ;

void *P0(void *arg){
	int j = i;

	for(j<=200){
		printf("%d" , i)
		j++;
	}
	sem_post(&S1);
}

void *P1(void *arg){
	int j = i*200+1

	sem_wait(&Si)
	while(j<=(i+1)*200){
		printf("%d" , i)
		j++;
	}
	i++;
	sem_post(&Si+1);
}


int main(void){
	pthread_t thread1, thread2;
	
	sem_init(&S1,0,0);
	sem_init(&S2,0,0);
	sem_init(&S3,0,0);
	sem_init(&S4,0,0);

	pthread_create(&thread1, NULL, P0, NULL);
	pthread_create(&thread2, NULL, P1, NULL);


	pthread_exit(NULL);	
	return(0);
}