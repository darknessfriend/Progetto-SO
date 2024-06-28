// Importo librerie necessarie ( classiche più quelle utilizzate dal professore )
#include <unistd.h>
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

// Struttura che rappresenta l'area riservata del file system.
// Contiene informazioni non di nostro interess, ma ne terrò comunque
// conto per completezza.
typedef struct __attribute__((__packed__)) ReservedArea{
    int placeholder1 = 0;
    int placeholder2 = 0;
    int placeholder3 = 0;
    int placeholder4 = 0;
} ReservedArea;

// Struttura che rappresenta un file ( il nostro file desriptor )
typdef struct{
    char filename[NAME_SIZE];
    int size;
    int start_block;
} FileHandle;

// Struttura che rappresenta una directory ( lista di file e directory )
typedf struct{
    char dirname[NAME_SIZE];
    FileHandle* files[];
    DirEntry* dirs[];
} DirEntry;

// Struttura che rappresenta un file system
typedef struct{
    int free_blocks[MAX_BLOCKS];
    ReservedArea reserved_area;
    DirEntry* root;
    DirEntry* current_dir;
} FileSystem;

// Definisco il nome delle funzioni che andrò ad implementare:
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
void seekFile(FileHandle* handle, uint32_t position);
// creazione di una directory
void createDir(const char* dirname);
// cancellazione di una directory
void eraseDir(const char* dirname);
// cambio di directory
void changeDir(const char* dirname);
// stampa del contenuto di una directory
void listDir();