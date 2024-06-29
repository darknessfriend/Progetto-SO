// Importo librerie necessarie ( classiche più quelle utilizzate dal professore )
// #include <unistd.h>
#include <stdint.h>
// #include <stdio.h>
// #include <assert.h>
// #include <string.h>
// #include <fcntl.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <stdlib.h>


// Inizio a definire le strutture che saranno utilizzate per la
// gestione del mio file system e le variabili globali che mi serviranno

// Numero massimo di blocchi di memoria che posso utilizzare
#define MAX_BLOCKS 1024
// Dimensione di un blocco
#define BLOCK_SIZE 512
// Grandezza massima del nome di un file/directory
#define NAME_SIZE 32
// Numero massimo di directory/file che posso avere in una directory
// ( Funzione di comodo per non dover allocare dinamicamente la memoria )
#define MAX_FILES 64

// Struttura che rappresenta l'area riservata del file system.
// Contiene informazioni non di nostro interess, ma ne terrò comunque
// conto per completezza.
typedef struct{
    int placeholder1;   // Offset 0
    int placeholder2;   // Offset 4
    int placeholder3;   // Offset 8
    int placeholder4;   // Offset 12
} ReservedArea;         // TOT: 16 bytes

// Struttura che rappresenta un file ( il nostro file desriptor )
typedef struct{
    char filename[NAME_SIZE];   // Offset 0
    int size;                   // Offset 32
    int start_block;            // Offset 36
} FileHandle;                   // TOT: 40 bytes

// Struttura che rappresenta una directory ( lista di file e directory )
typedef struct{
    char dirname[NAME_SIZE];                // Offset 0
    FileHandle* files[MAX_FILES];           // Offset 32
    struct DirEntry* dirs[MAX_FILES];       // Offset 32 + 64 * 8 = 544    
} DirEntry;                                 // TOT: 1056 bytes

// Struttura che rappresenta un file system
typedef struct{
    ReservedArea* reserved_area;                    // Offset 0
    int* FAT[MAX_BLOCKS];                           // Offset 8
    int start_index;                                // Offset 8 + 1024 * 8 = 8200
    // uint8 sono 8 bit ovvero 1 byte. Noi volevamo allocare 512 byte per blocco
    // quindi abbiamo bisogno di 512 uint8 per blocco.
    uint8_t data_blocks[MAX_BLOCKS][BLOCK_SIZE];    // Offset 8204
    DirEntry* root;                                 // Offset 8204 + 1024 * 512 = 532492
    DirEntry* current_dir;                          // Offset 532492 + 8 = 532500
} FileSystem;                                       // TOT: 532508 bytes

// Dichiaro le funzioni che andrò ad implementare:
// inizializzazione del file system
FileSystem* initFS();
// creazione di un file
FileHandle* createFile(const char* filename);
// cancellazione di un file
void eraseFile(const char* filename);
// scrittura di un file
void writeFile(FileHandle* handle, const void* buffer, size_t size);
// lettura di un file
void readFile(FileHandle* handle, void* buffer, size_t size);
// apertura di un file
void seekFile(FileHandle* handle, int position);
// creazione di una directory
void createDir(const char* dirname);
// cancellazione di una directory
void eraseDir(const char* dirname);
// cambio di directory
void changeDir(const char* dirname);
// stampa del contenuto di una directory
void listDir();