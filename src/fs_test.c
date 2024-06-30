// #include <unistd.h>
// #include <stdint.h>
#include <string.h>         // strlen
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
    printf("Creo file nella subdir:\n");
    FileHandle* file = createFile(fs,"file");
    writeFile(fs,file,"Hello, World!",13);
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
    writeFile(fs,file1,"[file1]Hello, World!",680);
    printf("Provo a chiedere tutta la memoria:\n");
    writeFile(fs,file2,"[file2]Hello, World!",524288);
    writeFile(fs,file2,"[file2]Hello, World!",6800);
    writeFile(fs,file3,"[file3]Hello, World!",5000);
    printFAT(fs,8);
    eraseFile(fs,"file1");
    eraseFile(fs,"file2");
    FileHandle* file4 = createFile(fs,"file4");
    printf("Testing con un file di dimensione maggiore di un blocco che si alloca da solo ( dimensione in input = a sizeof del buffer.\n");
    writeFile(fs,file4,"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur condimentum odio eget metus porta, quis rhoncus dolor accumsan. Fusce sed sagittis dui. Pellentesque ac sagittis elit. Maecenas efficitur viverra mi in gravida. In venenatis dui egestas tempus consectetur. Donec dignissim accumsan tortor, et sollicitudin est iaculis sit amet. Pellentesque non risus mattis, imperdiet metus aliquam, pellentesque odio. Maecenas in ligula fermentum, tristique magna ut, ornare lacus.Etiam a placerat erat. Suspendisse sapien lorem, vestibulum et pulvinar sit amet, ultrices quis sem. Aenean sed auctor nunc, vel viverra libero. Proin iaculis enim nec faucibus porta. Nullam fringilla dictum lacinia. Etiam interdum, ante in imperdiet posuere, lectus lectus pulvinar turpis, et molestie mi tellus vitae risus. Suspendisse at tristique nisi. Aliquam ipsum elit, sollicitudin nec tincidunt quis, mollis id eros. Nullam rutrum sapien non nulla suscipit, accumsan tristique orci fermentum. Phasellus et magna id nibh congue porttitor. Duis pharetra rhoncus enim quis hendrerit. Morbi blandit tristique risus, et efficitur neque bibendum et. Nunc ex massa, consectetur tristique pellentesque quis, suscipit sed orci.Sed congue malesuada sapien, quis porttitor sem ultricies vitae. Phasellus finibus erat vitae nulla condimentum, in sodales sem ornare. Suspendisse suscipit lacus quis risus sagittis, eget blandit felis varius. Mauris urna diam, eleifend at dictum sit amet, elementum congue odio. Etiam leo.",sizeof("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur condimentum odio eget metus porta, quis rhoncus dolor accumsan. Fusce sed sagittis dui. Pellentesque ac sagittis elit. Maecenas efficitur viverra mi in gravida. In venenatis dui egestas tempus consectetur. Donec dignissim accumsan tortor, et sollicitudin est iaculis sit amet. Pellentesque non risus mattis, imperdiet metus aliquam, pellentesque odio. Maecenas in ligula fermentum, tristique magna ut, ornare lacus.Etiam a placerat erat. Suspendisse sapien lorem, vestibulum et pulvinar sit amet, ultrices quis sem. Aenean sed auctor nunc, vel viverra libero. Proin iaculis enim nec faucibus porta. Nullam fringilla dictum lacinia. Etiam interdum, ante in imperdiet posuere, lectus lectus pulvinar turpis, et molestie mi tellus vitae risus. Suspendisse at tristique nisi. Aliquam ipsum elit, sollicitudin nec tincidunt quis, mollis id eros. Nullam rutrum sapien non nulla suscipit, accumsan tristique orci fermentum. Phasellus et magna id nibh congue porttitor. Duis pharetra rhoncus enim quis hendrerit. Morbi blandit tristique risus, et efficitur neque bibendum et. Nunc ex massa, consectetur tristique pellentesque quis, suscipit sed orci.Sed congue malesuada sapien, quis porttitor sem ultricies vitae. Phasellus finibus erat vitae nulla condimentum, in sodales sem ornare. Suspendisse suscipit lacus quis risus sagittis, eget blandit felis varius. Mauris urna diam, eleifend at dictum sit amet, elementum congue odio. Etiam leo. "));
    printFAT(fs,30);
    listDir(fs);
    printf("Provo ad effettuare una lettura del file4 ( lettura su più blocchi ):\n");
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
    printf("                                         BE EVIL WHEN TESTING                                      \n");
    printf("\n");
    printf("\n");   
    return 0;
}