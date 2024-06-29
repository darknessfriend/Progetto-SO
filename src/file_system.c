// Importo librerie necessarie ( classiche più quelle utilizzate dal professore )
// #include <unistd.h>
// #include <stdint.h>
// #include <assert.h>
// #include <fcntl.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <string.h>         // memset
#include <stdlib.h>         // malloc, free
#include <stdio.h>          // printf, perror
#include <sys/mman.h>       // mmap, munmap
#include "file_system.h"

// Tets print lo salvo un attimo qui
// printf("flag\n");
// fflush(stdout);

// Inizializzo filesystem
FileSystem* initFS() {

    // Inizializzo la struttura del file system
    FileSystem* fs = (FileSystem*) malloc(sizeof(FileSystem));
    // Gestisco l'errore
    if (fs == NULL) {
        perror("Error: malloc failed.\n");
        exit(1);
    }

    // Inizializzo l'area riservata
    ReservedArea* ra = (ReservedArea*) malloc(sizeof(ReservedArea));
    // Gestisco l'errore
    if (ra == NULL) {
        perror("Error: malloc failed.\n");
        exit(1);
    }
    ra->placeholder1 = 0;
    ra->placeholder2 = 0;
    ra->placeholder3 = 0;
    ra->placeholder4 = 0;
    fs->reserved_area = ra;

    // Inizializzo FAT table: -1 indica che il blocco è libero
    fs->FAT = (int*) malloc(MAX_BLOCKS * sizeof(int));
    // Gestisco l'errore
    if (fs->FAT == NULL) {
        perror("Error: malloc failed.\n");
        exit(1);
    }
    void* ret = memset(fs->FAT, -1, MAX_BLOCKS * sizeof(int));
    // Gestisco l'errore
    if (ret == NULL) {
        perror("Error: memset failed.\n");
        exit(1);
    }

    // Inizializzo l'indice di partenza
    fs->start_index = -1;

    // Inizializzo il data block
    // Mappo l'area di memoria puntata dal data block, alloco MAX_BLOCKS * BLOCK_SIZE bytes
    // PROT_READ | PROT_WRITE indica che la memoria è sia leggibile che scrivibile
    // MAP_PRIVATE | MAP_ANONYMOUS indica che la memoria è privata e non è associata ad alcun file, infatti
    // il file descriptor è -1 propio perchè non è associato ad alcun file. Non ho bisogno di memset perchè
    // con la flag MAP_ANONYMOUS la memoria è già inizializzata a 0.
    ret = mmap(fs->data_blocks, MAX_BLOCKS * BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    // Gestisco l'errore
    if (ret == MAP_FAILED) {
        perror("Error: mmap failed.\n");
        exit(1);
    }

    // Inizializzo la root directory
    DirEntry* root = createDir("/");
    fs->root = root;

    // Inizializzo la directory corrente
    fs->current_dir = root;

    return fs;
}

// Creazione di una directory
DirEntry* createDir(const char* dirname) {
    // Inizializzo la directory
    DirEntry* dir = (DirEntry*) malloc(sizeof(DirEntry));
    // Gestisco l'errore
    if (dir == NULL) {
        perror("Error: malloc failed.\n");
        exit(1);
    }
    // Copio il nome della directory
    char* ret = strncpy(dir->dirname, dirname, NAME_SIZE);
    // Gestisco l'errore
    if (ret == NULL) {
        perror("Error: strncpy failed.\n");
        exit(1);
    }
    // Inizializzo i file e le directory
    for (int i = 0; i < MAX_FILES; i++) {
        dir->files[i] = NULL;
        dir->dirs[i] = NULL;
    }
    return dir;
}

// Cancellazione del file system
void deleteFS(FileSystem* fs) {
    // Libero l'area riservata
    free(fs->reserved_area);
    // Libero la FAT table
    free(fs->FAT);
    // Libero il data block
    munmap(fs->data_blocks, MAX_BLOCKS * BLOCK_SIZE);
    // Libero la root directory
    free(fs->root);
    // Libero il file system
    free(fs);
}