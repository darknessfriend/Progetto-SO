// Numero massimo di blocchi di memoria che posso utilizzare
#define MAX_BLOCKS 1024
// Dimensione di un blocco
#define BLOCK_SIZE 512
// Grandezza massima del nome di un file/directory
#define NAME_SIZE 32
// Numero massimo di directory/file che posso avere in una directory
// ( Funzione di comodo per non dover allocare dinamicamente la memoria )
#define MAX_FILES 64
// Dimensione del file system
#define FILESYSTEM_SIZE 528384
// Calcola la dimensione totale
#define pointer_array_size MAX_BLOCKS * sizeof(void*)
#define total_size MAX_BLOCKS * BLOCK_SIZE
#define fat_size MAX_BLOCKS * sizeof(int)

// Struttura che rappresenta l'area riservata del file system.
// Contiene informazioni non di nostro interess, ma ne terrò comunque
// conto per completezza.
typedef struct{
    int placeholder1;   // Offset 0
    int placeholder2;   // Offset 4
    int placeholder3;   // Offset 8
    int placeholder4;   // Offset 12
} ReservedArea;         // TOT: 16 bytes

// Struttura che rappresenta un file ( il nostro file desriptor )
typedef struct{
    char filename[NAME_SIZE];   // Offset 0
    int size;                   // Offset 32
    int start_block;            // Offset 36
} FileHandle;                   // TOT: 40 bytes

// Struttura che rappresenta una directory ( lista di file e directory )
struct DirEntry{
    char dirname[NAME_SIZE];                // Offset 0
    int num_files;                          // Offset 32
    int num_dirs;                           // Offset 36
    FileHandle* files[MAX_FILES];           // Offset 40
    struct DirEntry* dirs[MAX_FILES];       // Offset 40 + 64 * 8 = 552
};                                          // TOT: 1064 bytes
// Definisco il tipo DirEntry (Necessario per la ricorsione)
struct DirEntry;
typedef struct DirEntry DirEntry;

// Struttura che rappresenta un file system
typedef struct{
    ReservedArea* reserved_area;                    // Offset 0
    int* FAT;                                       // Offset 8
    int free_blocks;                                // Offset 16
    void** data_blocks;                             // Offset 20
    DirEntry* root;                                 // Offset 20 + 1024 * 512 = 524308
    DirEntry* current_dir;                          // Offset 524308 + 8 = 524316
} FileSystem;                                       // TOT: 524324 bytes

// Dichiaro le funzioni che andrò ad implementare:
// inizializzazione del file system
FileSystem* initFS();
// creazione di un file
FileHandle* createFile(FileSystem* fs, const char* filename);
// cancellazione di un file
void eraseFile(FileSystem* fs, const char* filename);
// scrittura di un file
void writeFile(FileSystem* fs, FileHandle* handle, const void* buffer, size_t size);
// lettura di un file
void readFile(FileSystem* fs, FileHandle* handle);
// apertura di un file
void seekFile(FileHandle* handle, int position);
// creazione di una directory
void createDir(FileSystem* fs, const char* dirname);
// cancellazione di una directory
void eraseDir(FileSystem* fs, const char* dirname);
// cambio di directory
void changeDir(FileSystem* fs, const char* dirname);
// stampa del contenuto di una directory
void listDir(FileSystem* fs);
// cancellazione del file system
void deleteFS(FileSystem* fs);
// stampa della FAT
void printFAT(FileSystem* fs, int range);
// Trova un file all'interno del filesystem
FileHandle* findFile(FileSystem* fs, const char* filename);