#include <arpdfat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

/* Initializes a new file system by writing the
 * boot block, the fat table and the cluster for
 * file storage.*/
void __fat_init(FILE** fp){

	//discards contents of existing file or 
	//creates a new one.
	*fp = fopen("fat.part", "w+b"); 

	typedef uint16_t clu[256]; //1kb = 1024bytes = 512 uint_8 = 256 uint_16

	//reset file pointer 
	fseek(*fp, 0, SEEK_SET);

	//write boot cluster
	clu bb;
	memset(&bb, 0xbb, sizeof(bb));
	bb[255] = 0xbb;
	fwrite(&bb, sizeof(clu), 1, *fp);

	//write fat table
	clu fb;
	for(int i = 0; i < 256; i++) 
		fb[i] = 0xfffe;

	//fat_size = 8 * sizeof(clu)
	for(int i = 0; i < 8; i++)
		fwrite(&fb, sizeof(clu), 1, *fp);

	//root directory
	clu rd;
	memset(&rd, 0x00, sizeof(rd));
	fwrite(&rd, sizeof(clu), 1, *fp);

	//add remaining free space
	//fwrite(&rd, sizeof(clu), 4086, fp);

	//set fat ocuppied positions
	
}


/* Closes <fp> file pointer and enables the
 * is_it_time_to_quit flag, so the program
 * can terminate. */
void __fat_quit(FILE** fp, char* q){
	
	fclose(*fp);
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
