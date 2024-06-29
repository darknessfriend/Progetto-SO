// Importo librerie necessarie ( classiche più quelle utilizzate dal professore )
// #include <unistd.h>
// #include <stdint.h>
// #include <assert.h>
// #include <fcntl.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_system.h"

// Tets print lo salvo un attimo qui
// printf("flag\n");
// fflush(stdout);

// Inizializzo filesystem
FileSystem* initFS() {
    // Inizializzo la struttura del file system
    FileSystem* fs = (FileSystem*) malloc(sizeof(FileSystem));
    // Inizializzo l'area riservata
    ReservedArea* ra = (ReservedArea*) malloc(sizeof(ReservedArea));
    ra->placeholder1 = 0;
    ra->placeholder2 = 0;
    ra->placeholder3 = 0;
    ra->placeholder4 = 0;
    fs->reserved_area = ra;
    // Inizializzo i blocchi liberi
    
    memset(&((*fs).FAT), 0, sizeof(fs->FAT));
    return fs;
}