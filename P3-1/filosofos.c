#define N =  5 
#define LEFT =  (i+N-1)%N 
#define RIGHT =  (i+1)%N 
#define THINKING = 0 
#define HUNGRY = 1 
#define EATING = 2
typedef int semaphore; 
int state[N]; 
semaphore mutex = 1; 
semaphore s[N]; 
void philosopher(int i) { 
 while (TRUE) { 
 	think(); 
 	take_forks(i); 
 	eat(); 
 	put_forks(i); 
 }
}

void take_forks(int i){ 
 sem_wait(&mutex); 
 state[i] = HUNGRY;
 test(i); 
 sem_post(&mutex);
 sem_wait(&s[i]); 
}

void put_forks(i) 
 {sem_wait(&mutex); 
 state[i] = THINKING; 
 test(LEFT); 
 test(RIGHT); 
 sem_post(&mutex); 
}

void test(i) 
{ 
	if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
 state[i] = EATING; sem_post(&s[i]); 
	}
}

int main(void){
	pthread_t thread[N];
	int i = 0;

	sem_init(&mutex,0,1);

	for(i = 0;i<N;i++){
		sem_init(&s[i],0,1);
	}
	
	
	for(i = 0;i<lenght(s);i++){
		pthread_create(&thread[i], NULL, filosofo(i), NULL);
	}

	pthread_exit(NULL);	
	return(0);
}