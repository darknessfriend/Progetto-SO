// Importo librerie necessarie ( classiche più quelle utilizzate dal professore )
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
FileSystem fs;

// Inizializzo filesystem
void initFS() {
    // Inizializzo la struttura del file system
    memset(&fs, 0, sizeof(FileSystem));
    // Inizializzo l'area riservata
    ReservedArea reserved_area = {0, 0, 0, 0};
    fs.reserved_area = reserved_area;
    // Inizializzo i blocchi liberi
    memset(&fs.free_blocks, 0, sizeof(fs.free_blocks));
}