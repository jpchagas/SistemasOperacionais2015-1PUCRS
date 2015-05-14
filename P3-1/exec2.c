#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t S1, S2, S3

void *taskA(void *arg){
	
	printf("A");
	sem_post(&S1);
}

void *taskB(void *arg){

	printf("B");	
	sem_post(&S2);
}

void *taskC(void *arg){
	
	sem_wait(&S1);
	sem_wait(&S2);
	printf("C");
	sem_post(&S3);
	sem_post(&S3);
}

void *taskD(void *arg){

	sem_wait(&S3);
	printf("D");
}

void *taskE(void *arg){

	sem_wait(&S3);
	printf("E");
}

int main(void){
	pthread_t thread1, thread2, thread3, thread4, thread5;
	
	sem_init(&S1,0,0);
	sem_init(&S2,0,0);
	sem_init(&S3,0,0);


	pthread_create(&thread1, NULL, taskA, NULL);
	pthread_create(&thread2, NULL, taskB, NULL);
	pthread_create(&thread3, NULL, taskC, NULL);
	pthread_create(&thread4, NULL, taskD, NULL);
	pthread_create(&thread5, NULL, taskE, NULL);


	pthread_exit(NULL);	
	return(0);