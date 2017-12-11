CC=gcc
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=
EXEC=test

all: $(EXEC)

$(EXEC): $(EXEC).o move.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

$(EXEC).o: maintest.c move.h
	$(CC) -o $@ -c $< $(CFLAGS)

%.o : %.c #All of the .o files depend on the .c file of the same name.
	$(CC) -o $@ -c $< $(CFLAGS)
