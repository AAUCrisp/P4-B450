all: ClientStart ServerStart

ClientStart: ClientTest.c
	gcc -pthread -lconfig ClientTest.c -o ClientStart
	touch ClientStart

ServerStart: Control_Unit.c SocketFuncs.o
	gcc -pthread -lconfig Control_Unit.c SocketFuncs.o -o ServerStart
	touch ServerStart

SocketFuncs.o: Sockets.c Sockets_TX_RX.h
	gcc -c Sockets.c
	touch Sockets.c

clean:
	rm -f ClientStart ServerStart
	rm *.o
	@echo "clean completed!"
