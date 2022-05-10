all: ClientStart ServerStart SignalMonitoring

ClientStart: ClientTest.c
	gcc -pthread ClientTest.c -o ClientStart
	touch ClientStart

ServerStart: Control_Unit.c Sockets.o RSSI.o
	gcc -pthread Control_Unit.c Sockets.o RSSI.o -o ServerStart
	touch ServerStart

SignalMonitoring: SignalMonitoring.c shm_read.o Sockets.o
	gcc -c -pthread SignalMonitoring.c shm_read.o Sockets.o -o SignalMonitoring
	touch SignalMonitoring.c

Sockets.o: Sockets.c  Sockets_TX_RX.h
	gcc -c Sockets.c 
	touch Sockets.c

RSSI.o: RSSI.c Sockets_TX_RX.h
	gcc -c RSSI.c
	touch RSSI.c

shm_write.o: shm_write.c shm_write_read.h
	gcc -c shm_write.c
	touch shm_write.c 

shm_read.o: shm_read.c shm_write_read.h
	gcc -c shm_read.c 
	touch shm_read.c 

clean:
	rm -f ClientStart ServerStart SignalMonitoring
	rm *.o
	@echo "clean completed!"
