// #include <unistd.h>
// #include <stdint.h>
// #include <assert.h>
#include <string.h>         // strlen
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
    int** var = (int**) fs->data_blocks;
    printf("Test (dovrebbe ritornare 0): %d;\n",var[0][0]);
    printf("Test (dovrebbe ritornare /): %s;\n",fs->root->dirname);
    printf("Test (dovrebbe ritornare /): %s;\n",fs->current_dir->dirname);
    printf("----------- Test initFS completati ----------\n");
    // // Creo una directory come subdirectory della root
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
    FileHandle* file1 = createFile(fs,"file1");
    printf("Creo un file con lo stesso nome ( va in errore gestito e continua l'esecuzione ).\n");
    FileHandle* file2 = createFile(fs,"file1");
    printf("Creo 2 file nella directory corrente:\n");
    file2 = createFile(fs,"file2");
    FileHandle* file3 = createFile(fs,"file3");
    listDir(fs);
    printFAT(fs,3);
    writeFile(fs,file1,"Hello, World!",680);
    writeFile(fs,file2,"Hello, World!",6800);
    writeFile(fs,file3,"Hello, World!",5000);
    printFAT(fs,8);
    eraseFile(fs,"file1");
    eraseFile(fs,"file2");
    FileHandle* file4 = createFile(fs,"file4");
    writeFile(fs,file4,"Hello, World!",strlen("Hello, World!"));
    printFAT(fs,30);
    listDir(fs);
    printf("Provo ad effettuare una lettura del file4:\n");
    readFile(fs,file4);
    // // Dealloco il file system
    printf("Dealloco il filesystem.\n");
    printf("Done. Exiting ...\n");
    deleteFS(fs);
    printf("                                                                                                    \n");
    printf("                         ...                                                .                       \n");
    printf("                         ..*@:.                                          .@#..                      \n");
    printf("                           .@@@@..                                    .-@@%%.                        \n");
    printf("                          ..@@#@@@..                                 .@@#@@:                        \n");
    printf("                           %%@**#*@@..                               @@##*#@@.                       \n");
    printf("                           @@######@@.                          ..%%@##**#*@@.                       \n");
    printf("                           @##*#**##@@..                       .:@@**#*####@.                       \n");
    printf("                           @*#####***@@@.                     .#@@#####**#*@..                      \n");
    printf("                           @###**%%*##@@@@..                   @@@@##**#####@..                      \n");
    printf("                           @@##*#####@@@*@..               .=@#@@@##*#####%%@                        \n");
    printf("                           .@###**#**#@@#*@@.              %%@*#@@###*#*#*%%@.                        \n");
    printf("                             @@#*####***###%%@             %%@#%%###***####%%@..                        \n");
    printf("                         ......@@@*#*######@@#          .#@@@@***####%%@@.....                       \n");
    printf("                         .@+.......-----.. ...          . .. ...---=..... ..@.                      \n");
    printf("                          @@@=....                                      .+@@@#                      \n");
    printf("                         .@.:@@@..                                ... %%@@%%:.@..                     \n");
    printf("                         .@#.:.-@@@%%...                        .. %%@@@.:...@                       \n");
    printf("                           @:....@.#@@@@@@::..          ...:@%%@@@@...@..::@..                      \n");
    printf("                           .@....@.:.:...@@@@@@@@@@@@@@@@@@@@....:..:.@:::@...                      \n");
    printf("                            .@@.:@.......-:..@..:..:.....:%%=.....:::..@.%%@...                       \n");
    printf("                            . .@-@.:..:.:..:.@.:....:...:.%%@...:..:.:.@@:                           \n");
    printf("                               ..@@..:..:...:@.:::......:.%%@.....:..#@-.                            \n");
    printf("                                  ..@@.:.....@..-......:..@@...::%%@#..                               \n");
    printf("                                      .@@@@::@:.....:.....@@*%%@@..                                  \n");
    printf("                                          ...-@@@@@@@@@@@-.....                                     \n");
    printf("\n");
    printf("\n");
    printf("                                          BE EVIL WHEN TESTING                                      \n");
    return 0;
}