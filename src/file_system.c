#include <string.h>         // memset
#include <stdlib.h>         // malloc, free
#include <stdio.h>          // printf, perror
#include <sys/mman.h>       // mmap, munmap
#include "file_system.h"

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
    fs->free_blocks = 1024;

    // Inizializzo il data block
    // Mappo l'area di memoria puntata dal data block, alloco MAX_BLOCKS * sizeof(void*) bytes
    // PROT_READ | PROT_WRITE indica che la memoria è sia leggibile che scrivibile
    // MAP_PRIVATE | MAP_ANONYMOUS indica che la memoria è privata e non è associata ad alcun file, infatti
    // il file descriptor è -1 propio perchè non è associato ad alcun file. Non ho bisogno di memset perchè
    // con la flag MAP_ANONYMOUS la memoria è già inizializzata a 0.
    fs->data_blocks = (void**) mmap(NULL, MAX_BLOCKS * sizeof(void*), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    // Gestisco l'errore
    if (fs->data_blocks == MAP_FAILED) {
        perror("Error: mmap failed.\n");
        exit(1);
    }
    for (int i = 0; i < MAX_BLOCKS; i++) {
        // Mappo l'area di memoria puntata dal data block, alloco BLOCK_SIZE bytes
        fs->data_blocks[i] = (void*) mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        // Gestisco l'errore
        if (*fs->data_blocks == MAP_FAILED) {
            perror("Error: mmap failed.\n");
            exit(1);
        }
    }

    // Setto la current dir a NULL per creare la root directory con la funzione createDir
    fs->current_dir = NULL;

    // Inizializzo la root directory e la setto a current dir
    createDir(fs, "/");

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
    if (fs->current_dir != NULL) {
        for (int i = 0; i < fs->current_dir->num_dirs; i++) {
            if (strcmp(fs->current_dir->dirs[i]->dirname, dirname) == 0) {
                printf("Error: directory already exists.\n");
                return;
            }
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
    // Se la directory corrente è NULL allora la directory creata è la root

    if (fs->current_dir == NULL){
        fs->current_dir = dir;
        fs->root = dir;
    }
    else{
        dir->dirs[0] = fs->current_dir;
        dir->num_dirs++;
    }

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

// Cancellare un file
void eraseFile(FileSystem* fs, const char* filename){
    // Controllo parametri in input
    if (fs == NULL || filename == NULL) {
        printf("Error: invalid input parameters in eraseFile.\n");
        return;
    }
    void* ret;
    // Controllo se il file esiste
    for (int i = 0; i < fs->current_dir->num_files; i++) {
        if (strcmp(fs->current_dir->files[i]->filename, filename) == 0) {
            // Libero il blocco
            int index = fs->current_dir->files[i]->start_block;
            while(fs->FAT[index] != -2){
                int next = fs->FAT[index];
                ret = memset(fs->data_blocks[index],0,BLOCK_SIZE);
                // Gestisco l'errore
                if (ret == NULL) {
                    perror("Error: memset failed.\n");
                    exit(1);
                }
                fs->free_blocks++;
                fs->FAT[index] = -1;
                index = next;
            }

            // Libero il blocco finale
            fs->FAT[index] = -1;

            // Libero il file handler
            free(fs->current_dir->files[i]);

            // Sposto i file successivi
            for (int j = i; j < fs->current_dir->num_files - 1; j++) {
                fs->current_dir->files[j] = fs->current_dir->files[j + 1];
            }

            // Diminuisco il numero di file
            fs->current_dir->num_files--;

            printf("File %s deleted.\n", filename);
            return;
        }
    }
    printf("Error: file does not exist.\n");
}

// Funzione ricorsiva di supporto per la cancellazione delle directory
void destroyDIR(FileSystem* fs, DirEntry* dir){
    // Controllo parametri in input
    if (fs == NULL || dir == NULL) {
        printf("Error: invalid input parameters in destroyDIR.\n");
        return;
    }
    void* ret;
    // Libero i file
    for (int i = 0; i < dir->num_files; i++) {
        int index = dir->files[i]->start_block;
        while(fs->FAT[index] != -2){
            int next = fs->FAT[index];
            ret = memset(fs->data_blocks[index],0,BLOCK_SIZE);
            // Gestisco l'errore
            if (ret == NULL) {
                perror("Error: memset failed.\n");
                exit(1);
            }
            fs->free_blocks++;
            fs->FAT[index] = -1;
            index = next;
        }
        // Libero il blocco finale
        fs->FAT[index] = -1;
        printf("File %s deleted.\n", dir->files[i]->filename);
        // Libero il file
        free(dir->files[i]);
    }
    dir->num_files = 0;
    // Libero le subdirectory
    for (int i = 1; i < dir->num_dirs; i++) {
        destroyDIR(fs,dir->dirs[i]);
    }
    // Libero la directory
    printf("Directory %s deleted.\n", dir->dirname);
    free(dir);
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
            destroyDIR(fs,fs->current_dir->dirs[i]);
            // Sposto le directory successive
            if (strcmp(dirname,"/") != 0) {    
                for (int j = i; j < fs->current_dir->num_dirs - 1; j++) {
                    fs->current_dir->dirs[j] = fs->current_dir->dirs[j + 1];
                }
                fs->current_dir->num_dirs--;
            }
            return;
        }
    }
    printf("[eraseDir]Error: directory does not exist.\n");
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
            fs->FAT[i] = -2; // -2 indica che il blocco è occupato (leggi documentazione in README.md)
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

// Scrivere un file
void writeFile(FileSystem* fs, FileHandle* handle, const void* buffer, size_t size){
    // Controllo parametri in input
    if (fs == NULL || handle == NULL || buffer == NULL) {
        printf("Error: invalid input parameters in writeFile.\n");
        return;
    }
    // Controllo se la dimensione del file è sufficiente
    if (size > fs->free_blocks * BLOCK_SIZE) {
        printf("Error: file size too large.\n");
        return;
    }

    // Scrivo il primo blocco di memoria
    void* ret = memcpy(fs->data_blocks[handle->start_block], buffer, BLOCK_SIZE);
    // Gestisco l'errore
    if (ret == NULL) {
        perror("Error: memcpy failed.\n");
        exit(1);
    }
    // Aggiorno la dimensione del file
    size-=BLOCK_SIZE;
    // Aggiorno il numero di blocchi liberi
    fs->free_blocks--;
    // Scrivo il file sui blocchi di memoria liberi:
    // Memorizzo l'indice del blocco precedente in modo da poterlo collegare al blocco successivo
    // nella FAT table.
    int previous_block = handle->start_block;
    // Memorizzo il numero di blocchi allocati spostare il puntatore del buffer in input e calcolare
    // la dimensione della memoria allocata alla fine.
    int allocated_blocks = 1;
    // Memorizzo un indice per scorrere la FAT table.
    for(int i = 0; i < MAX_BLOCKS; i++){
        if ((int)size <= 0) {
            break;
        }
        // Ho trovato un blocco libero nella fat table
        if(fs->FAT[i] == -1){
            // Setto il blocco come occupato (leggi documentazione in README.md)
            fs->FAT[i] = -2;
            // Scrivo il blocco di memoria nel data block
            ret = memcpy(fs->data_blocks[i], buffer + allocated_blocks*BLOCK_SIZE, BLOCK_SIZE);
            // Gestisco l'errore
            if (ret == NULL) {
                perror("Error: memcpy failed.\n");
                exit(1);
            }
            // Collego il blocco precedente al blocco successivo
            fs->FAT[previous_block] = i;
            // Aggiorno l'indice del blocco precedente
            previous_block = i;
            // Aggiorno il numero di blocchi allocati
            allocated_blocks++;
            // Aggiorno la dimensione del file ancora da allocare
            size-=BLOCK_SIZE;
            // Aggiorno il numero di blocchi liberi
            fs->free_blocks--;
        }
    }

    // Alla fine della nostra operazione, l'elemento successivo dell'ultimo blocco allocato
    // deve essere -2. Lo ricerco e lo setto.
    if ( fs->free_blocks > 0 ) {
        for (int i = 0; i < MAX_BLOCKS; i++) {
            if (fs->FAT[i] == -1) {
                fs->FAT[previous_block] = i;
                // In questa implementazione andrò a perdere un pò di memoria ma è
                // necessario settare il blocco di memoria puntato dal previous ad un valore speciale
                // in modo che non possa venire allocato nessun file in quel blocco altrimenti potrebbe
                // creare problemi durante la lettura del file. (leggi documentazione in README.md)
                fs->FAT[i] = -2;
                break;
            }
        }
    }
    else {
        printf("Error: the file was allocated successfully, however there are no more blocks available!\n");
    }

    handle->size = strlen((char*)buffer);
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
    // Libero tutte le strutture DirEntry ed i FileHandle allocato dinamicamente:
    eraseDir(fs,"/");
    // Libero la FAT table
    free(fs->FAT);
    // Libero il data block
    int ret;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        ret = munmap(fs->data_blocks[i], BLOCK_SIZE);
        // Gestisco l'errore
        if (ret == -1) {
            perror("Error: munmap failed.\n");
            exit(1);
        }
    }
    ret = munmap(fs->data_blocks, MAX_BLOCKS * sizeof(void*));
    // Gestisco l'errore
    if (ret == -1) {
        perror("Error: munmap failed.\n");
        exit(1);
    }
    // Libero il file system
    free(fs);
}

// Stampa la FAT table (usata per il testing e debugging)
void printFAT(FileSystem* fs, int range) {
    // Controllo parametri in input
    if (fs == NULL) {
        printf("Error: invalid input parameters in printFAT.\n");
        return;
    }

    int rangex;
    if (range == -1){
        rangex = MAX_BLOCKS;
    }
    else{
        rangex = range;
    }
    printf("---------- FAT Table ----------\n");
    for(int i = 0; i <= rangex; i++){
        printf("FAT[%d] = %d\n",i,fs->FAT[i]);
    }
    printf("---------- End FAT Table ----------\n");
}

// Funzione per la lettura di un file
void readFile(FileSystem* fs, FileHandle* handle) {
    // Controllo parametri in input
    if (fs == NULL || handle == NULL) {
        printf("Error: invalid input parameters in readFile.\n");
        return;
    }
    // Controllo se il file esiste
    for (int i = 0; i < fs->current_dir->num_files; i++) {
        if (strcmp(fs->current_dir->files[i]->filename, handle->filename) == 0) {
            // Leggo il file
            int index = handle->start_block;
            for ( int j = 0; j < handle->size / BLOCK_SIZE+1; j++) {
                printf("%s",(char*) fs->data_blocks[index]);
                index = fs->FAT[index];
            }
            printf("\n");
            return;
        }
    }
    printf("[readFile]Error: file does not exist.\n");
}

// Funzione per la modifica della posizione di lettura di un file
void seekFile(FileHandle* handle, int position) {
    // Controllo parametri in input
    if (handle == NULL) {
        printf("Error: invalid input parameters in seekFile.\n");
        return;
    }
    // Controllo se la posizione è valida
    if (position < 0 || position > handle->size) {
        printf("Error: invalid position.\n");
        return;
    }
    // Modifico la posizione di lettura
    handle->start_block = position / BLOCK_SIZE;
}

// Trova un file nel filesystem
FileHandle* findFile(FileSystem* fs, const char* filename){
    // Controllo parametri in input
    if (fs == NULL || filename == NULL) {
        printf("Error: invalid input parameters in findFile.\n");
        return NULL;
    }
    // Controllo se il file esiste
    for (int i = 0; i < fs->current_dir->num_files; i++) {
        if (strcmp(fs->current_dir->files[i]->filename, filename) == 0) {
            return fs->current_dir->files[i];
        }
    }
    printf("Error: file does not exist.\n");
    return NULL;
}
