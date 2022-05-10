all: ClientStart ServerStart SignalMonitoring

ClientStart: ClientTest.c
	gcc -pthread ClientTest.c -o ClientStart
	touch ClientStart

ServerStart: Control_Unit.c Sockets.o RSSI.o shm_write.o shm_read.o
	gcc -pthread Control_Unit.c Sockets.o RSSI.o shm_write.o shm_read.o -o ServerStart
	touch ServerStart

SignalMonitoring: SignalMonitoring.c RSSI.o shm_write.o shm_read.o Sockets.o
	gcc -pthread SignalMonitoring.c RSSI.o shm_write.o shm_read.o Sockets.o
	touch SignalMonitoring

#Sockets: Sockets.c Sockets.o shm_read.o
#	gcc -c Sockets.c Sockets.o shm_read.o
#	touch Sockets

Sockets.o: Sockets.c Sockets_TX_RX.h shm_write_read.h
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
