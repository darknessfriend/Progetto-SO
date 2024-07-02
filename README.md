# Progetto-SO
Realizing pseudo-FAT file system

### Prerequisites
To run the program, you will need to install "make". On linux, it is possible with the following command:
```bash
sudo apt install make
```

To run the program just type: 
```bash
sudo apt install make
./mount
```

# Folders:
- src: Source folder, contains all the files that are used for the implementation of the FAT file system;
- utils: Contains useful resources that I used for this project;
- .gitignore: List of file extensions that will not be pushed in my github project;
# Files:
- fat_map.png: Rappresentation of the FAT32 structure that I'm developing (src: "http://elm-chan.org/docs/rc/fat_map.png");
- file_system.h: Implementation of the data structures and definition of global variables;
- file_system.c: Implementation of the function defined in the header file;
- fs_test.c: It contains the main function, and it's the file used to test the whole filesystem;
- makefile: File used to compile the other files;
- Assignment.txt: Contains the assignment of the project;

# Source:
- http://elm-chan.org/docs/fat_e.html;
- https://it.wikipedia.org/wiki/File_Allocation_Table;
- https://github.com/dyerw/filesystem;
- https://gitlab.com/grisetti/sistemi_operativi_2023_24/-/blob/main/slides/os_12_file_system.pdf;
- https://chat.openai.com;

# Documentation
-2 Value:
The value -2, used on file creation and on file writing. It's a special value. It means that a block
cannot be overwritten, but doesn't either point a valid memory zone so if you are reading a file and
the next FAT entry is -2, you reached the EOF.
