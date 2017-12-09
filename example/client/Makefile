export LD_LIBRARY_PATH=/src/ev3dev-c/lib
all:
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c tester.c -o tester.o
	arm-linux-gnueabi-gcc tester.o -Wall -o tester -lev3dev-c.a 
	arm-linux-gnueabi-gcc -std=gnu99 -W -Wall -c robotclient.c -o robotclient.o
	arm-linux-gnueabi-gcc robotclient.o -Wall  -o robotclient -lm libev3dev-c -lbluetooth
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c i2c.c -o i2c.o
	arm-linux-gnueabi-gcc i2c.o -Wall -lm -lev3dev-c -o i2c

i2c:
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c i2c.c -o i2c.o
	arm-linux-gnueabi-gcc i2c.o -Wall -lm -lev3dev-c -o i2c
	arm-linux-gnueabi-gcc robotclient.o -Wall  -o robotclient -lm libev3dev-c.a -lbluetooth
	./i2c

client:
	arm-linux-gnueabi-gcc -std=gnu99 -W -Wall -c robotclient.c -o robotclient.o
	arm-linux-gnueabi-gcc robotclient.o -Wall -lm -lev3dev-c -lbluetooth -o robotclient
	./robotclient

run:
	./tester
