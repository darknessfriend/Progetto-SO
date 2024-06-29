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
    printf("Test (should return -1):\n%d;\n",fs->FAT[104]);
    printf("Test (should return /):\n%s;\n",fs->root->dirname);
    printf("Test (should return /):\n%s;\n",fs->current_dir->dirname);
    deleteFS(fs);
    printf("Done. Exiting ...\n");
    return 0;
}