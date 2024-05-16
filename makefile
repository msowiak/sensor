CFLAGS=-c -Wall -O2
LIBS = -lm -lbme280 -lpthread -lpaho-mqtt3c

all: bmetest

bmetest: main.o
	$(CC) main.o $(LIBS) -o bmetest

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	rm *.o bmetest
