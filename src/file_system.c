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
    if (fs == NULL) {
        perror("Error: malloc failed\n");
        exit(1);
    }
    // Inizializzo l'area riservata
    ReservedArea* ra = (ReservedArea*) malloc(sizeof(ReservedArea));
    if (ra == NULL) {
        perror("Error: malloc failed\n");
        exit(1);
    }
    ra->placeholder1 = 0;
    ra->placeholder2 = 0;
    ra->placeholder3 = 0;
    ra->placeholder4 = 0;
    fs->reserved_area = ra;
    // Inizializzo FAT table: -1 indica che il blocco è libero
    *(fs->FAT) = (int*) malloc(MAX_BLOCKS * sizeof(int));
    if (*(fs->FAT) == NULL) {
        perror("Error: malloc failed\n");
        exit(1);
    }
    for (int i = 0; i < MAX_BLOCKS; i++) {
        (*fs->FAT)[i] = -1;
    }
    // Inizializzo l'indice di partenza
    fs->start_index = -1;
    memset(&((*fs).FAT), 0, sizeof(fs->FAT));
    return fs;
}