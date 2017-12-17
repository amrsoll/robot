CC=arm-linux-gnueabi-gcc
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=-I/src/ev3dev-c/source/ev3/ -lm /src/ev3dev-c/lib/libev3dev-c.a
EXEC=robotclient

all: $(EXEC)

$(EXEC): $(EXEC).o gsyst.o map.o path.o spot.o servercom.o
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

$(EXEC).o: robotclient.c gsyst.h map.h path.h spot.h servercom.h
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

map.o: map.c gsyst.h
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

path.o: path.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

spot.o: spot.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

servercom.o: servercom.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

gsyst.o: gsyst.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)

.PHONY: clean mrproper #tells that clean and mrproper are not filenames

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
