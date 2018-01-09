# @Author: Axel_Soll <amrsoll>
# @Date:   08/01/2018
# @Email:  axel.soll@telecom-paristech.fr
# @Last modified by:   amrsoll
# @Last modified time: 09/01/2018



ARCH=$(shell uname -m)

ifeq ($(ARCH),x86_64) #this means we are on the docker platform
	CC=arm-linux-gnueabi-gcc
	CFLAGS=-W -Wall -ansi -pedantic
	LDFLAGS=-I/src/ev3dev-c/source/ev3/ -lm /src/ev3dev-c/lib/libev3dev-c.a
	BT=
endif
ifeq ($(ARCH),armv5tejl) #this means we are compiling on the brick
	CC=gcc
	CFLAGS=-W -Wall -ansi -pedantic
	LDFLAGS=
	BT=-lbluetooth
endif
#delete LDFLAGS when on the robot
EXEC=robotclient

all: $(EXEC)

$(EXEC): $(EXEC).o gsyst.o #path.o spot.o servercom.o #map.o
	$(CC) $^ $(LDFLAGS) -o $(EXEC)

$(EXEC).o: robotclient.c gsyst.h constants.m #path.h spot.h servercom.h #map.h
	$(CC) -o $@ -c $< $(LDFLAGS)

###### compile the test files for each module

ball: ball_test.o ball.o
	$(CC) $^ $(LDFLAGS) -o ball
ball_test.o: ball_test.c ball.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

map: map_test.o gsyst.o map.o
	$(CC) $^ $(LDFLAGS) -o map
map_test.o: map_test.c map.h gsyst.h constants.m classes.m
	$(CC) -o $@ -c $< $(LDFLAGS)

path: path_test.o path.o
	$(CC) $^ $(LDFLAGS) -o path
path_test.o: path_test.c path.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

spot: spot_test.o spot.o
	$(CC) $^ $(LDFLAGS) -o spot
spot_test.o: spot_test.c spot.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

servercom: servercom_test.o servercom.o
	$(CC) $^ $(LDFLAGS) -o servercom $(BT)
servercom_test.o: servercom_test.c servercom.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS) $(BT)

gsyst: gsyst_test.o gsyst.o
	$(CC) $^ $(LDFLAGS) -o gsyst
gsyst_test.o: gsyst_test.c gsyst.h constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)

####### compile the .o files for each module
#If they need a special flag, then add it here
servercom.o: servercom.c constants.m
	$(CC) -o $@ -c $< $(LDFLAGS) $(BT)
#If not, then this is where it is compiles
%.o: %.c constants.m
	$(CC) -o $@ -c $< $(LDFLAGS)


.PHONY: clean mrproper #tells that clean and mrproper are not filenames

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
	rm -rf ball
	rm -rf map
	rm -rf spot
	rm -rf path
	rm -rf servercom
	rm -rf gsyst


	# make the test files for the modules
