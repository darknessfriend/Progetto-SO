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
// gestione del mio file system.

// Numero massimo di blocchi di memoria che posso utilizzare
#define MAX_BLOCKS 1024
// Dimensione di un blocco
#define BLOCK_SIZE 512
// Grandezza massima del nome di una directory
#define NAME_SIZE 32

// Struttura che rappresenta un blocco di memoria
typdef struct{
    char name[NAME_SIZE];
    int size;
    int start_block;
    int is_dir;
} Block;

// Struttura che rappresenta un file system
typedef struct{
    Block blocks[MAX_BLOCKS];
    int n_blocks;
    int first_free_block;
} FileSystem;