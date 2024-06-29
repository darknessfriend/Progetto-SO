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
// Controllo parametri:
// Controllo parametri in input
// if (fs == NULL || dirname == NULL) {
//     printf("Error: invalid input parameters in createDir.\n");
//     return;
// }

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
    // Inizializzo aree di memoria riservata
    ra->placeholder1 = 0;
    ra->placeholder2 = 0;
    ra->placeholder3 = 0;
    ra->placeholder4 = 0;
    // Assegno la memoria riservata al file system
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
    // Inizializzo la directory
    fs->root = (DirEntry*) malloc(sizeof(DirEntry));
    // Gestisco l'errore
    if (fs->root == NULL) {
        perror("Error: malloc failed.\n");
        exit(1);
    }
    // Copio il nome della directory
    char* ret1 = strcpy(fs->root->dirname,"/");
    // Gestisco l'errore
    if (ret1 == NULL) {
        perror("Error: strcpy failed.\n");
        exit(1);
    }
    // Inizializzo numero di file e directory
    fs->root->num_files = 0;
    fs->root->num_dirs = 0;
    // Inizializzo i file e le directory
    for (int i = 0; i < MAX_FILES; i++) {
        fs->root->files[i] = NULL;
        fs->root->dirs[i] = NULL;
    }

    // Inizializzo la directory corrente
    fs->current_dir = fs->root;

    return fs;
}

// Creazione di una directory
void createDir(FileSystem* fs, const char* dirname) {

    // Controllo parametri in input
    if (fs == NULL || dirname == NULL) {
        printf("Error: invalid input parameters in createDir.\n");
        return;
    }

    // Controllo se la directory esiste già
    for (int i = 0; i < fs->current_dir->num_dirs; i++) {
        if (strcmp(fs->current_dir->dirs[i]->dirname, dirname) == 0) {
            printf("Error: directory already exists.\n");
            return;
        }
    }

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

    // Inizializzo numero di file e directory
    dir->num_files = 0;
    dir->num_dirs = 0;

    // Inizializzo i file e le directory
    for (int i = 0; i < MAX_FILES; i++) {
        dir->files[i] = NULL;
        dir->dirs[i] = NULL;
    }

    // Aggiungo la directory generatrice alla directory creata
    dir->dirs[0] = fs->current_dir;
    dir->num_dirs++;

    // Aggiungo la directory alla directory corrente
    fs->current_dir->dirs[fs->current_dir->num_dirs] = dir;
    fs->current_dir->num_dirs++;
}

// Stampa il contenuto della directory corrente
void listDir(FileSystem* fs){

    // Controllo parametri in input
    if (fs == NULL) {
        printf("Error: invalid input parameters in createDir.\n");
        return;
    }

    printf("---------- List dir ----------\n");
    printf("Directory corrente: %s\n", fs->current_dir->dirname);
    printf("Files:\n");
    for(int i = 0; i < fs->current_dir->num_files; i++){
        printf("%s\n", fs->current_dir->files[i]->filename);
    }
    printf("Directories:\n");
    for(int i = 0; i < fs->current_dir->num_dirs; i++){
        printf("%s\n", fs->current_dir->dirs[i]->dirname);
    }
    printf("---------- End list ----------\n");
}

// Cambiare la directory corrente del filesystem
void changeDir(FileSystem* fs, const char* dirname){
    // Controllo parametri in input
    if (fs == NULL || dirname == NULL) {
        printf("Error: invalid input parameters in changeDir.\n");
        return;
    }
    // Controllo se la directory esiste
    for (int i = 0; i < fs->current_dir->num_dirs; i++) {
        if (strcmp(fs->current_dir->dirs[i]->dirname, dirname) == 0) {
            fs->current_dir = fs->current_dir->dirs[i];
            printf("Directory changed to %s.\n", fs->current_dir->dirname);
            return;
        }
    }
    printf("Error: directory does not exist.\n");
}

// Eliminare una directory
void eraseDir(FileSystem* fs, const char* dirname){
    // Controllo parametri in input
    if (fs == NULL || dirname == NULL) {
        printf("Error: invalid input parameters in eraseDir.\n");
        return;
    }
    // Controllo se la directory esiste
    for (int i = 0; i < fs->current_dir->num_dirs; i++) {
        if (strcmp(fs->current_dir->dirs[i]->dirname, dirname) == 0) {
            // Libero la directory
            free(fs->current_dir->dirs[i]);
            // Sposto le directory successive
            for (int j = i; j < fs->current_dir->num_dirs - 1; j++) {
                fs->current_dir->dirs[j] = fs->current_dir->dirs[j + 1];
            }
            fs->current_dir->num_dirs--;
            printf("Directory %s deleted.\n", dirname);
            return;
        }
    }
    printf("Error: directory does not exist.\n");
}

// Creare un file
FileHandle* createFile(FileSystem* fs, const char* filename){
    // Controllo parametri in input
    if (fs == NULL || filename == NULL) {
        printf("Error: invalid input parameters in createFile.\n");
        return NULL;
    }

    // Controllo se il file esiste già
    for (int i = 0; i < fs->current_dir->num_files; i++) {
        if (strcmp(fs->current_dir->files[i]->filename, filename) == 0) {
            printf("Error: file already exists.\n");
            return NULL;
        }
    }

    // Inizializzo il file
    FileHandle* file = (FileHandle*) malloc(sizeof(FileHandle));
    // Gestisco l'errore
    if (file == NULL) {
        perror("Error: malloc failed.\n");
        exit(1);
    }

    // Copio il nome del file
    char* ret = strcpy(file->filename,filename);
    // Gestisco l'errore
    if (ret == NULL) {
        perror("Error: strcpy failed.\n");
        exit(1);
    }

    // Inizializzo la dimensione del file
    file->size = 0;

    // Cerco un blocco libero
    int block = -1;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (fs->FAT[i] == -1) {
            block = i;
            fs->FAT[i] = i;
            break;
        }
    }
    // Controllo se ho trovato un blocco libero
    if (block == -1) {
        printf("Error: no free blocks available.\n");
        return NULL;
    }

    // Assegno il blocco al file
    file->start_block = block;

    // Aggiungo il file alla directory corrente
    fs->current_dir->files[fs->current_dir->num_files] = file;
    fs->current_dir->num_files++;

    return file;
}

// Cancellazione del file system
void deleteFS(FileSystem* fs) {
    // Controllo parametri in input
    if (fs == NULL) {
        printf("Error: invalid input parameters in createDir.\n");
        return;
    }
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

