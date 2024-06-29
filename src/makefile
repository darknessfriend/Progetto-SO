# Compilatore
CC = gcc
CCOPTS=-std=gnu99 -g -Wall 

# File oggetto
OBJS=fs_test.o file_system.o

# Nome del binario
BINS=mount

# File header
HEADERS=file_system.h 

all: $(BINS)

# Creazione dei file oggetto
%.o:	%.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<

# Compilazione del binario filesystem
mount:	fs_test.c file_system.c
	$(CC) -$(CCOPTS) -o $@ $^

# Elimina i file oggetto e l'eseguibile
clean:
	rm -rf *.o
	rm -rf $(BINS)

.phony: clean all