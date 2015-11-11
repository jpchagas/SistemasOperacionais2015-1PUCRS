#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpdfat.h>
#include <string.h>

#define ACT_QUIT 0x00
#define ACT_INIT 0x01
#define ACT_LOAD 0x02
#define ACT_LS 0x03
#define ACT_MKDIR 0x04
#define ACT_CREATE 0x05
#define ACT_UNLINK 0x06
#define ACT_WRITE 0x07
#define ACT_APPEND 0x08
#define ACT_READ 0x09
#define ACT_UNK 0x0A

/* Parse user input into function parameters */
void set_params(const char*, int*, char*, char*, char*);

/* Main routine. Initialize internal
 * structures and loops over the shell. */
int main(int argc, char** argv){

	/* Stores the pointer to out fat.part file. */
	FILE* fat_ptr;

	/* Loads fat.part file */
	fat_ptr = NULL;

	/* Menu variables */ 
	int menu_choice = 0xf4;
	char cmd_buffer[255];
	char arg00[255];
	char arg01[255];
	char F_ACE;

	//system("clear");
	fprintf(stderr, "Welcome to SFAT - Segmentation Fault Ahead Terminal\n");

	//loop control
	char is_it_time_to_quit = 0;

	//presents menu
	do{
		fprintf(stderr, "[arpd@sisop]$ ");	

		memset(cmd_buffer, 0, 255);
		fgets (cmd_buffer, 255, stdin);

		set_params(cmd_buffer, &menu_choice, arg00, arg01, &F_ACE);

		switch(menu_choice){
			case ACT_QUIT:
				if(arg00[0] == '\0') __fat_quit(&fat_ptr, &is_it_time_to_quit);
				else fprintf(stderr, "INFO: Closes the file system and exits.\n\tUsage: quit\n");
				break;
			case ACT_INIT:
				if(arg00[0] == '\0') __fat_init(&fat_ptr);
				else fprintf(stderr, "INFO: Initilizes a new file system whitin \"fat.part\" file.\n\tUsage: init\n");
				break;
			case ACT_LOAD:
				if(arg00[0] == '\0') __fat_load(&fat_ptr);
				else fprintf(stderr, "INFO: Loads an existing file system from \"fat.part\" file.\n\tUsage: load\n");
				break;
			case ACT_LS:
				if(arg00[0] != '\0' && arg01[0] == '\0') __fat_ls(&fat_ptr, arg00);
				else fprintf(stderr, "INFO: Lists the content of <dn> directory.\n\tUsage: ls <dn>\n");
				break;
			case ACT_MKDIR:
				if(arg00[0] != '\0' && arg01[0] == '\0') __fat_mkdir(&fat_ptr, arg00);
				else fprintf(stderr, "INFO: Creates a directory named <dn>.\n\tUsage: mkdir <dn>\n");
				break;
			case ACT_CREATE:
				if(arg00[0] != '\0' && arg01[0] == '\0') __fat_create(&fat_ptr, arg00);
				else fprintf(stderr, "INFO: Creates a new file named <fn>.\n\tUsage: create <fn>\n");
				break;
			case ACT_UNLINK:
				if(arg00[0] != '\0' && arg01[0] == '\0') __fat_unlink(&fat_ptr, arg00);
				else fprintf(stderr, "INFO: Deletes an existing file or empty directory named <en>.\n\tUsage: unlink <en>\n");
				break;
			case ACT_WRITE: 
				if(arg00[0] != '\0' && arg01[0] != '\0' && !F_ACE) __fat_write(&fat_ptr, arg00, arg01);
				else fprintf(stderr, "INFO: Writes the contents of <str> into an existing file <fn>.\n\tUsage: write <str> <fn>\n");
				break;
			case ACT_APPEND:
				if(arg00[0] != '\0' && arg01[0] != '\0' && !F_ACE) __fat_append(&fat_ptr, arg00, arg01);
				else fprintf(stderr, "INFO: Insert the contents of <str> at the end of existing file <fn>.\n\tUsage: append <str> <fn>\n");
				break;
			case ACT_READ:
				if(arg00[0] != '\0' && arg01[0] == '\0') __fat_read(&fat_ptr, arg00);
				else fprintf(stderr, "INFO: Reads the content of an existing file <fn>.\n\tUsage: read <fn>\n");
				break;
			default: fprintf(stderr, "Unrecognized command.\n"); break;
		}
	}while(!is_it_time_to_quit);
}

/* Parser <cmd> into a command <choice>, passing
 * <arg00> and <arg01> as argumment. */
void set_params(const char* cmdb, int* choice, char* tk0, char* tk1, char* un){

	int i, seek;

	/* parses command */
	char cmd[50];

	memset(cmd, 0, sizeof(char) * 50);
	memset(tk0, 0, sizeof(char) * 255);
	memset(tk1, 0, sizeof(char) * 255);

	/* gets first token */
	for(i = 0; i < 49; i++){
		if(cmdb[i] == ' ' || cmdb[i] == '\n' || cmdb[i] == '\0'){
			break;
		}else{
			cmd[i] = cmdb[i];
		}
	}

	/* fix string terminal */
	if(cmd[i] != '\0') cmd[i] = '\0';
	
	/* start second token on next position */
	i++;

	/* iterates through second token */
	for(seek = 0; i < 250; i++){
		if(cmdb[i] == ' ' || cmdb[i] == '\n' || cmdb[i] == '\0'){
			break;
		}else{
			tk0[seek] = cmdb[i];
		}
		seek++;
	}
	
	/* fix string terminal */
	if(tk0[seek] != '\0') tk0[seek] = '\0';

	/* starts third token on next position */
	i++;

	/* iterates through second token */
	for(seek = 0; i < 250; i++){
		if(cmdb[i] == ' ' || cmdb[i] == '\n' || cmdb[i] == '\0'){
			break;
		}else{
			tk1[seek] = cmdb[i];
		}
		seek++;
	}
	
	/* fix string terminal */
	if(tk1[seek] != '\0') tk1[seek] = '\0';

	/* advance to next position */
	i++;

	/* parameter_exceeded flag */
	*un = (cmdb[i] != '\0');

	/* compares with available commands */
	if(!strcmp(cmd, "quit"))
		*choice = ACT_QUIT;
	else if(!strcmp(cmd, "init"))
		*choice = ACT_INIT;
	else if(!strcmp(cmd, "ls"))
		*choice = ACT_LS;
	else if(!strcmp(cmd, "mkdir"))
		*choice = ACT_MKDIR;
	else if(!strcmp(cmd, "create"))
		*choice = ACT_CREATE;
	else if(!strcmp(cmd, "unlink"))
		*choice = ACT_UNLINK;
	else if(!strcmp(cmd, "write"))
		*choice = ACT_WRITE;
	else if(!strcmp(cmd, "read"))
		*choice = ACT_READ;
	else if(!strcmp(cmd, "append"))
		*choice = ACT_APPEND;
	else if(!strcmp(cmd, "load"))
		*choice = ACT_LOAD;
	else
		*choice = ACT_UNK;
}
