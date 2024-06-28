// Importo librerie necessarie ( classiche più quelle utilizzate dal professore )
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>


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
typedef struct __attribute__((__packed__)) ReservedArea{
    int placeholder1;
    int placeholder2;
    int placeholder3;
    int placeholder4;
} ReservedArea;

// Struttura che rappresenta un file ( il nostro file desriptor )
typedef struct{
    char filename[NAME_SIZE];
    int size;
    int start_block;
} FileHandle;

// Struttura che rappresenta una directory ( lista di file e directory )
typedef struct{
    char dirname[NAME_SIZE];
    struct DirEntry* dirs[MAX_FILES];
    FileHandle* files[MAX_FILES];
} DirEntry;

// Struttura che rappresenta un file system
typedef struct{
    ReservedArea reserved_area;
    // uint8 sono 8 bit ovvero 1 byte. Noi volevamo allocare 512 byte per blocco
    // quindi abbiamo bisogno di 512 uint8 per blocco.
    uint8_t free_blocks[MAX_BLOCKS][BLOCK_SIZE];
    DirEntry* root;
    DirEntry* current_dir;
} FileSystem;

// Dichiaro le funzioni che andrò ad implementare:
// inizializzazione del file system
void initFS();
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