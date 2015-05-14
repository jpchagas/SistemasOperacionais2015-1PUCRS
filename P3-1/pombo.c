#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define N=20

int contaPostIt=0;
semaforo mutex=1; 
semaforo cheia=0; 
semaforo enchendo=N; 


void *usuario(void *arg){
	while(true){
		sem_wait(&enchendo);
		sem_wait(&mutex);
		printf("cola post it);
		contaPostIt++;
		if (contaPostIt == N)
			sem_post(&cheia);
		sem_post(&mutex);
	}
}

void *pombo(void *arg){
	int i;
	while(true){
		sem_wait(&cheia);
		sem_wait(&mutex);
		printf("pombo levando a mochila e voltando...");
		contaPostIt = 0;
		for(i = 0; i < N; i++)
			sem_post(&enchendo);
		sem_post(&mutex);
	}
}

int main(void){
	pthread_t thread1, thread2;
	
	sem_init(&sem_msg,0,0);
	sem_init(&sem_env,0,1);



	pthread_create(&thread1, NULL, usuario, NULL);
	pthread_create(&thread2, NULL, pombo, NULL);



	pthread_exit(NULL);	
	return(0);
}