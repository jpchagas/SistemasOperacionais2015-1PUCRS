/* 8 clusters da tabela FAT, 4096 entradas de 16 bits = 8192 bytes*/
uint16_t fat[4096];

/* entrada de diretorio, 32 bytes cada */
typedef struct{
uint8_t filename[18];
uint8_t attributes;
uint8_t reserved[7];
uint16_t first_block;
uint32_t size;
} dir_entry_t;

/* diretorios (incluindo ROOT), 32 entradas de diretorio
com 32 bytes cada = 1024 bytes */
dir_entry_t dir[32];

/* bloco de dados, 1024 bytes */
uint8_t data_block[1024];
