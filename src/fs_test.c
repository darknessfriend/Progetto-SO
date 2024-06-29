// #include <unistd.h>
// #include <stdint.h>
// #include <assert.h>
// #include <string.h>
// #include <fcntl.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "file_system.h"

// Variabili globali
FileSystem* fs;

int main(){
    // Inizializzo filesystem
    fs = initFS();
    // printf("Offset of start_index: %ld\n", offsetof(DirEntry, dirs));
    printf("Test (should return -1):\n%d;\n", (*fs->FAT)[0]);
    printf("Done. Exiting ...\n");
    return 0;
}