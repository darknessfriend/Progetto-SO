#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "file_system.h"

// Variabili globali
FileSystem files;

int main(){
    // Inizializzo filesystem
    initFS();
    printf("Done. Exiting ...\n");
    return 0;
}