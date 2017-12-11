CC=gcc
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=-I./../ev3dev-c/source/ev3
EXEC=test

all: $(EXEC)

$(EXEC): $(EXEC).o move.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

$(EXEC).o: maintest.c move.h
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

%.o : %.c #All of the .o files depend on the .c file of the same name.
	$(CC) -o $@ -c $< $(CFLAGS)
