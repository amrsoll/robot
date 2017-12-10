CC=gcc
EV3LIB=-I./../ev3dev-c/source/ev3 -O2 -std=gnu99 
#use whenever you need the EV3 libraries
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=
EXEC=robotclient

all: $(EXEC)

$(EXEC): $(EXEC).o gsyst.o map.o path.o spot.o servercom.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

$(EXEC).o: robotclient.c gsyst.h map.h path.h spot.h servercom.h
	$(CC) -o $@ -c $< $(CFLAGS)

map.o: map.c gsyst.h
	$(CC) -o $@ -c $< $(CFLAGS)

path.o: path.c
	$(CC) -o $@ -c $< $(CFLAGS)

spot.o: spot.c
	$(CC) -o $@ -c $< $(CFLAGS)

servercom.o: servercom.c
	$(CC) -o $@ -c $< $(CFLAGS)

gsyst.o: gsyst.c
	$(CC) $(EV3LIB) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper #tells that clean and mrproper are not filenames

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
