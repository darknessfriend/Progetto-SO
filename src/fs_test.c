// #include <unistd.h>
// #include <stdint.h>
// #include <assert.h>
// #include <string.h>
// #include <fcntl.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <stdlib.h>
#include <stddef.h>         // offsetof
#include <stdio.h>          // printf
#include "file_system.h"

// Variabili globali
FileSystem* fs;

int main(){
    // Inizializzo filesystem
    fs = initFS();
    // Test sull'inizializzazione del file system
    printf("---------- Test sull'inizializzazione del file system ----------\n");
    printf("Test (dovrebbe ritornare -1): %d;\n",fs->FAT[104]);
    printf("Test (dovrebbe ritornare 0): %d;\n",fs->data_blocks[0][0]);
    printf("Test (dovrebbe ritornare /): %s;\n",fs->root->dirname);
    printf("Test (dovrebbe ritornare /): %s;\n",fs->current_dir->dirname);
    printf("----------- Test initFS completati ----------\n");
    // Creo una directory come subdirectory della root
    printf("---------- Test generale file system ----------\n");
    printf("Creo una directory come subdirectory della root:\n");
    createDir(fs,"subdir");
    printf("Creo una directory con lo stesso nome ( va in errore gestito e continua l'esecuzione ).\n");
    createDir(fs,"subdir");
    listDir(fs);
    printf("Cambio directory corrente in subdir:\n");
    changeDir(fs,"subdir");
    printf("Creo 2 directory come subdirectory della subdir:\n");
    createDir(fs,"subdir2");
    createDir(fs,"subdir3");
    listDir(fs);
    printf("Ne elimino una:\n");
    eraseDir(fs,"subdir2");
    listDir(fs);
    printf("Torno alla root:\n");
    changeDir(fs,"/");
    listDir(fs);
    printf("Creo un file nella directory corrente:\n");
    createFile(fs,"file1");
    // Dealloco il file system
    deleteFS(fs);
    printf("Done. Exiting ...\n");
    return 0;
}