##########################################################################################################################################################################################
#							Algoritmo de  Peterson																				 #
##########################################################################################################################################################################################

#include <stdio.h>

int nivel[n];
int ultimoEntrar[n];

void lock(void *arg){
	long int i;

	i = (long int)arg;
	for (l = 0; count < (n); count++) {
		nivel[i] = count;
		ultimoEntrar[l] = i;
		while(ultimoEntrar[l]=i and  )
	}
}

void unlock(void *arg){
	long int i;

	i = (long int)arg;
	flags[i] = -1;
}
