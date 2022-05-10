all: ClientStart ServerStart

ClientStart: ClientTest.c
	gcc -pthread ClientTest.c -o ClientStart
	touch ClientStart

ServerStart: Control_Unit.c Sockets.o RSSI.o
	gcc -pthread Control_Unit.c Sockets.o RSSI.o -o ServerStart
	touch ServerStart

Sockets.o: Sockets.c  Sockets_TX_RX.h
	gcc -c Sockets.c 
	touch Sockets.c

RSSI.o: RSSI.c Sockets_TX_RX.h
	gcc -c RSSI.c
	touch RSSI.c

clean:
	rm -f ClientStart ServerStart
	rm *.o
	@echo "clean completed!"
