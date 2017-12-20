CC=arm-linux-gnueabi-gcc
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=-I/src/ev3dev-c/source/ev3/ -lm /src/ev3dev-c/lib/libev3dev-c.a
EXEC=robotclient

all: $(EXEC)

$(EXEC): $(EXEC).o gsyst.o #path.o spot.o servercom.o #map.o
	$(CC) $^ $(LDFLAGS) -o $(EXEC)

$(EXEC).o: robotclient.c gsyst.h constants.m #path.h spot.h servercom.h #map.h
	$(CC) -o $@ -c $< $(LDFLAGS)

###### compile the test files for each module

ball: ball_test.o ball.o
	$(CC) $^ $(LDFLAGS) -o ball.o
	rm -rf ball.o
ball_test.o: ball_test.c ball.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

map: map_test.o map.o
	$(CC) $^ $(LDFLAGS) -o map.o
	rm -rf map.o
map_test.o: map_test.c map.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

path: path_test.o path.o
	$(CC) $^ $(LDFLAGS) -o path.o
	rm -rf path.o
path_test.o: path_test.c path.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

spot: spot_test.o spot.o
	$(CC) $^ $(LDFLAGS) -o spot.o
	rm -rf spot.o
spot_test.o: spot_test.c spot.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

servercom: servercom_test.o servercom.o
	$(CC) $^ $(LDFLAGS) -o servercom.o
	rm -rf servercom.o
servercom_test.o: servercom_test.c servercom.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

gsyst: gsyst_test.o gsyst.o
	$(CC) $^ $(LDFLAGS) -o gsyst.o
	rm -rf gsyst.o
gsyst_test.o: gsyst_test.c gsyst.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

####### compile the .o files for each module

ball.o: ball.c gsyst.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

map.o: map.c gsyst.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

path.o: path.c constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

spot.o: spot.c constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

servercom.o: servercom.c constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

gsyst.o: gsyst.c constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)


.PHONY: clean mrproper #tells that clean and mrproper are not filenames

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

	# make the test files for the modules
