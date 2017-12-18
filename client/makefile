CC=arm-linux-gnueabi-gcc
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=-I/src/ev3dev-c/source/ev3/ -lm /src/ev3dev-c/lib/libev3dev-c.a
EXEC=robotclient

all: $(EXEC)

$(EXEC): $(EXEC).o gsyst.o #path.o spot.o servercom.o #map.o
	$(CC) $^ $(LDFLAGS) -o $(EXEC)

$(EXEC).o: robotclient.c gsyst.h constants.m #path.h spot.h servercom.h #map.h
	$(CC) -o $@ -c $< $(LDFLAGS)

ball.o: ball.c gsyst.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

# map.o: map.c gsyst.h constants.m
# 	$(CC) -o $@ -c $< $(LDFLAGS)

# path.o: path.c constants.m
# 	$(CC) -o $@ -c $< $(LDFLAGS)
#
# spot.o: spot.c constants.m
# 	$(CC) -o $@ -c $< $(LDFLAGS)
#
# servercom.o: servercom.c constants.m
# 	$(CC) -o $@ -c $< $(LDFLAGS)

gsyst.o: gsyst.c constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

.PHONY: clean mrproper #tells that clean and mrproper are not filenames

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
