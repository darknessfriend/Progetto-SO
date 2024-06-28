# Compilatore
CC = gcc
CCOPTS=-std=gnu99 -g -Wall 

OBJS=fs_test.o file_system.o

BINS=filesystem

HEADERS=file_system.h 

all: $(BINS)

%.o:	%.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<

filesystem:	fs_test.c file_system.c
	$(CC) -$(CCOPTS) -o $@ $^

clean:
	rm -rf *.o

.phony: clean all