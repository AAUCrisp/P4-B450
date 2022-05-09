all: ClientStart ServerStart

ClientStart: ClientTest.c
	gcc -pthread -lconfig ClientTest.c -o ClientStart
	touch ClientStart

ServerStart: Control_Unit.c Sockets.o 
	gcc -pthread -lconfig Control_Unit.c Sockets.o -o ServerStart
	touch ServerStart

Sockets.o: Sockets.c RSSI.c Sockets_TX_RX.h
	gcc -c Sockets.c RSSI.c
	touch Sockets.c RSSI.c

clean:
	rm -f ClientStart ServerStart
	rm *.o
	@echo "clean completed!"
