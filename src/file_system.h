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
// Grandezza massima del nome di una directory
#define NAME_SIZE 32

// Struttura che rappresenta un blocco di memoria
typdef struct{
    char name[NAME_SIZE];
    int size;
    int start_block;
    int is_dir;
} Block;

// Struttura che rappresenta un file ( il nostro file desriptor )
typdef struct{
    int id;
    int pos;
    int size;
    int mode;
} FileHandle;

// Struttura che rappresenta l'area riservata del file system
// contiene informazioni non di nostro interess, ma ne terrò comunque
// conto per completezza.
typedef struct __attribute__((__packed__)) ReservedArea{
    
} ReservedArea;

// Struttura che rappresenta un file system
typedef struct{
    ReservedArea reserved_area;
    Block blocks[MAX_BLOCKS];
    int n_blocks;
    int first_free_block;
} FileSystem;