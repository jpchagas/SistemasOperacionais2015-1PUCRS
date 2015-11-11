#include <stdio.h>

#define CLUSTER_SIZE 1028
#define FAT_CLUSTERS 8
#define FAT_ENTRY_SIZE 2

/* Closes partition file and enables the
 * is_it_time_to_quit flag. */
void __fat_quit(FILE**, char*);

/* Erase existing file system and initilize
 * it from the scratch. */
void __fat_init(FILE**);

/* Load existing file system from fat.part
 * file. */
void __fat_load(FILE**);

/* Display information about a directory. */
void __fat_ls(FILE**, char*);

/* Creates a new directory. */
void __fat_mkdir(FILE**, char*);

/* Creates a new file. */
void __fat_create(FILE**, char*);

/* Removes a file or empty directory. */
void __fat_unlink(FILE**, char*);

/* Writes content to a file. */
void __fat_write(FILE**, char*, char*);

/* Appends content to the end of a file. */
void __fat_append(FILE**, char*, char*);

/* Display contents of a file. */
void __fat_read(FILE**, char*);
