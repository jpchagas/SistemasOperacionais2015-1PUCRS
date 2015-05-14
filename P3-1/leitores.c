#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int rc = 0; //Número de leitores
int wc = 0; //Número de escritores, apenas um escritor de cada vez pode ter acesso aos
//dados compartilhados
sem_t mutex_rc // Protege o acesso à variável rc
sem_t mutex_wc //Protege o acesso à variável wc
sem_t mutex //Impede que + do que 1 leitor tente entrar na região crítica
bool w_db = false //Indica a um escritor se este pode ter acesso aos dados
bool r_db =false //Permite que um processo leitor tente entrar na sua região crítica

void *leitores(void *arg){
while (TRUE){
	sem_wait(mutex);
	sem_wait(r_db);
	sem_wait(mutex_rc);
	rc++;
	if (rc == 1)
		sem_wait(w_db);
		sem_post(mutex_rc);
		sem_post(r_db);
		sem_post(mutex);
…
//Leitura dos dados
…
		sem_wait(mutex_rc);
		rc--;
	if (rc == 0)
		sem_post(w_db);
	sem_post(mutex_rc); 
	}
}

void *escritores(void *arg){
while (TRUE){
	sem_wait(mutex_wc);
	wc++;
		if (wc == 1)
			sem_wait(r_db);
			sem_post(mutex_wc);
			sem_wait(w_db)
…
//Escrita
…
			sem_post(w_db)
			sem_wait(mutex_wc);
			wc--;
		if (wc == 0)
			sem_post(r_db);
			sem_post(mutex_wc);
	}
}

int main(void){
	pthread_t thread1, thread2;
	
	sem_init(&mutex_wc,0,1);
	sem_init(&mutex_rc,0,1);
	sem_init(&mutex,0,1);


	pthread_create(&thread1, NULL, reader, NULL);
	pthread_create(&thread2, NULL, writer, NULL);



	pthread_exit(NULL);	
	return(0);
}