#include <arpdfat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define NUM_BLOCKS 4096
#define CLU_SIZE 1024
#define FAT_SIZE 1024 * 8

//fat em memória. tem o tamanho de 8 blocos ( 8 * tamanho do cluster)
//é uma lista de 2048 palavras de 8 bits ( 8 bits = 1 char = 1 uint8)
uint16_t fat[NUM_BLOCKS];

//cluter que serve de buffer para carregar e salvar cluster no disco.
//tem o tamanho de 256 palavras e pode ser ~castado~ direto para a 
//struct de diretório, ou interpretado direto, no caso de arquivo.

uint8_t clu[CLU_SIZE];

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

	//agora criamos os blocos da fat. inicialmente a fat tem
	//todos os valores em 0xfffe. mudaremos estes posteriormente.
	for(int i = 0; i < CLU_SIZE; i += 2){
		clu[i]   = 0xff;
		clu[i+1] = 0xfe;
	}

	//gravamos 8 clusters no disco, pois esta pedindo no enunciado
	for(int i = 0; i < 8; i++)
		fwrite(clu, CLU_SIZE, 1, *fp);

	//criamos o diretorio raiz. com valor inicial de 0x11. 
	//tambem mudaremos isto depois. na segunda linha, gravamos
	//ele no disco
	memset(clu, 0x11, CLU_SIZE);
	fwrite(clu, CLU_SIZE, 1, *fp);

	//até aqui temos BOOT_BLOCK + 8 FAT_BLOCKS + 1 ROOT_DIR, ou seja,
	//10 blocos. Agora inicializamos todo o resto do disco com o 
	//valor 0x00 a partir do bloco 10 até o bloco 4095. Total de 4086
	//blocos de dados.
	memset(clu, 0x00, CLU_SIZE);
	for(int i = 10; i < NUM_BLOCKS; i++)
		fwrite(clu, CLU_SIZE, 1, *fp);

	//agora ajustamos as entradas da fat para apontarem para os lugares
	//corretos. fat[0] = boot, fat[1..9] = fat, fat[10] = root.
	fseek(*fp, CLU_SIZE, SEEK_SET); //tamanho do cluster + inicio de arquivo.
	fread(fat, FAT_SIZE, 1, *fp); //carrega a fat do disco em memória.

	//seta boot 	
	fat[0] = 0xfffd; //boot

	//seta fat
	for(int i = 1; i < 9; i++)
		fat[i] = 0xfffe;

	//root dir (EOF)
	fat[10]= 0xffff;

	//todos as outras entradas apontam para lugar nenhum
	for(int i = 11; i < NUM_BLOCKS; i++)
		fat[i] = 0x0000;	

	//recoloca ponteiro no inicio da FAT apos a leitura
	fseek(*fp, CLU_SIZE, SEEK_SET);

	//salva fat em disco
	fwrite(fat, FAT_SIZE, 1, *fp);

}


/* Closes <fp> file pointer and enables the
 * is_it_time_to_quit flag, so the program
 * can terminate. */
void __fat_quit(FILE** fp, char* q){
	
	//fclose(*fp);
	*q = 1; //is_it_time_to_quit	
	fprintf(stderr, "Bye.\n");
}

void __fat_load(FILE** fp){
	
	//reads existing file
	*fp = fopen("fat.part","r+b");

	//did it load?
	if(!*fp){
		fprintf(stderr, "Unable to load existing partition. Use <init> to create");
		fprintf(stderr, " a new one\n");
		return;
	}

	fprintf(stderr, "Partition file loaded succefully.\n");
}

void __fat_ls(FILE** fp, char* dir){}
void __fat_mkdir(FILE** fp, char* dir){}
void __fat_create(FILE** fp, char* path){}
void __fat_unlink(FILE** fp, char* path){}

void __fat_write(FILE** fp, char* str, char* path){}
void __fat_append(FILE** fp, char* str, char* path){}

void __fat_read(FILE** fp, char* path){}
