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
    printf("Offset of FAT: %ld\n", offsetof(FileSystem, start_index));
    printf("Test (should return 0):\n%d;\n", fs->reserved_area->placeholder1);
    printf("Done. Exiting ...\n");
    return 0;
}