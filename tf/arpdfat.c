#include <arpdfat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define NUM_BLOCKS 4096
#define DIR_SIZE 32
#define DIR_COUNT 32
#define CLU_SIZE 1024
#define FAT_SIZE 1024 * 8

//fat em memória. tem o tamanho de 8 blocos ( 8 * tamanho do cluster)
//é uma lista de 2048 palavras de 8 bits ( 8 bits = 1 char = 1 uint8)
uint16_t fat[NUM_BLOCKS];

//cluter que serve de buffer para carregar e salvar cluster no disco.
//tem o tamanho de 256 palavras e pode ser ~castado~ direto para a 
//struct de diretório, ou interpretado direto, no caso de arquivo.
uint8_t clu[CLU_SIZE];

//um cluster é composto de 32 entradas de diretório com 32 bytes cada.
//por isso o cluster tem 1024 bytes (32 * 32 = 1024). para acessar os
//campos da entrada de diretório, utilizamos a struct abaixo.
typedef struct {
	uint8_t nome[18];
	uint8_t atributo;
	uint8_t x_x; //inutil
	uint8_t inicio; //onde esta o primeiro bloco
	uint32_t tamanho; //total de bytes, pra saber até onde ler
} entry;

//pra nao ter que ficar dando cast em todo o programa, usamos uma union pra 
union{
	entry dir_data[DIR_SIZE];    //o mesmo espaco de memoria pode ser um diretorio
	uint8_t raw_data[CLU_SIZE];  //ou um conteudo de arquivo. no segundo caso o
                                     //arquivo é tratado como um array de bytes.
}

//funcao que inicializa a fat. inicialmente, o disco esta vazio, entao
//adicionamos o BOOT_BLOCK, mais 8 blocos de FAT, mais o directorio 
//ROOT, todos manualmente. 
void __fat_init(FILE** fp){

	//abre que armazena o disco. se o arquivo nao existir, 
	//este é criado automaticamente. se existir, apenas é
	//carregado.
	*fp = fopen("fat.part", "w+b"); 

	//posiciona o ponteiro no inicio do arquivo. ele deveria
	//vir no inicio, mas resetamos para garantir que aconteça. 
	fseek(*fp, 0, SEEK_SET); // 0 + inicio = inicio

	//criamos um bloco (cluster) para o BOOT_BLOCK. para isso,
	//enchemos o buffer com o valor 0xbb. na segunda linha, 
	//escremos o buffer no disco
	memset(clu, 0xbb, CLU_SIZE);
	fwrite(clu, CLU_SIZE, 1, *fp);

	//apontamento do BOOT dentro da FAT
	fat[0] = 0xfffd; //boot

	//apontamento dos blocos da FAT dentro da própria FAT
	for(int i = 1; i < 9; i++)
		fat[i] = 0xfffe;

	//apontamento do diretório ROOT dentro da FAT
	fat[9]= 0xffff;

	//todas as outras entradas da FAT apontam para lugar nenhum
	for(int i = 10; i < NUM_BLOCKS; i++)
		fat[i] = 0x0000;	

	//escreve a fat em disco
	fwrite(fat, FAT_SIZE, 1, *fp);

	//criamos o diretorio raiz. com valor inicial de 0x11. 
	//mudaremos isto depois. na segunda linha, gravamos
	//ele no disco.
	memset(clu, 0x11, CLU_SIZE);
	fwrite(clu, CLU_SIZE, 1, *fp);

	//até aqui temos BOOT_BLOCK + 8 FAT_BLOCKS + 1 ROOT_DIR, ou seja,
	//10 blocos. Agora inicializamos todo o resto do disco com o 
	//valor 0x00 a partir do bloco 10 até o bloco 4095. Total de 4086
	//blocos de dados.
	memset(clu, 0x00, CLU_SIZE);
	for(int i = 10; i < NUM_BLOCKS; i++)
		fwrite(clu, CLU_SIZE, 1, *fp);

	//mostra fat na tela
	for(int i = 0; i < NUM_BLOCKS; i++){
		fprintf(stderr, "%04x ", fat[i]);
		if((i+1) % 16 == 0) fprintf(stderr, "\n");
	}
}


//liga a flag de saida e termina o programa.
void __fat_quit(FILE** fp, char* q){
	
	*q = 1; 	
	fprintf(stderr, "Bye.\n");
}

//carrega registros da FAT existentes no disco. o programa nao
//garante que estes registros existam.
void __fat_load(FILE** fp){
	
	//abre o arquivo para leitura e edicao
	*fp = fopen("fat.part","r+b");

	//checa erros na abertura do arquivo
	if(!*fp){
		fprintf(stderr, "Unable to load existing partition. Use <init> to create");
		fprintf(stderr, " a new one\n");
		return;
	}

	//posiciona ponteiro no inicio da fat do disco
	fseek(*fp, CLU_SIZE, SEEK_SET); //fat esta 1 bloco depois do inicio

	//carrega dados dentro do vetor FAT
	fread(fat, FAT_SIZE, 1, *fp);

	//mostra fat na tela
	for(int i = 0; i < NUM_BLOCKS; i++){
		fprintf(stderr, "%04x ", fat[i]);
		if((i+1) % 16 == 0) fprintf(stderr, "\n");
	}
}

void __fat_ls(FILE** fp, char* dir){}
void __fat_mkdir(FILE** fp, char* dir){}
void __fat_create(FILE** fp, char* path){}
void __fat_unlink(FILE** fp, char* path){}

void __fat_write(FILE** fp, char* str, char* path){}
void __fat_append(FILE** fp, char* str, char* path){}

void __fat_read(FILE** fp, char* path){}
