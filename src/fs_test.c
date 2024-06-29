// #include <unistd.h>
// #include <stdint.h>
// #include <assert.h>
// #include <string.h>
// #include <fcntl.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <stdlib.h>
#include <stdio.h>
#include "file_system.h"

// Variabili globali
FileSystem fs;

int main(){
    // Inizializzo filesystem
    initFS(fs);
    printf("Done. Exiting ...\n");
    return 0;
}