all: MakeClean Git ClientStart ServerStart SignalMonitoring

MakeClean: 
	make clean

Git:
	git pull

ClientStart: ClientTest.c
	gcc -pthread ClientTest.c -o ClientStart
	touch ClientStart

ServerStart: Control_Unit.c Sockets.o RSSI.o shm_write.o shm_read.o
	gcc -pthread Control_Unit.c Sockets.o RSSI.o shm_write.o shm_read.o -o ServerStart -lrt
	touch ServerStart

SignalMonitoring: SignalMonitoring.c RSSI.o shm_write.o shm_read.o Sockets.o
	gcc -pthread SignalMonitoring.c RSSI.o shm_write.o shm_read.o Sockets.o -o SignalMonitoring -lrt
	touch SignalMonitoring

Sockets.o: Headers/Sockets.c Headers/Sockets.h Headers/shm_write_read.h
	gcc -c Headers/Sockets.c -lrt
	touch Headers/Sockets.c

RSSI.o: Headers/RSSI.c Headers/Sockets.h
	gcc -c Headers/RSSI.c -lrt
	touch Headers/RSSI.c

shm_write.o: Headers/shm_write.c Headers/shm_write_read.h
	gcc -c Headers/shm_write.c -lrt
	touch Headers/shm_write.c 

shm_read.o: Headers/shm_read.c Headers/shm_write_read.h
	gcc -c Headers/shm_read.c -lrt
	touch Headers/shm_read.c 


# Dependencies 
Dependencies = $(wildcard */*.c)

test: Libraries.c $(Dependencies)
	gcc -c Libraries.c $(Dependencies) -lrt
	touch Libraries.c


# TESTING SHIT!
# SignalMonitoring: SignalMonitoring.c $(Dependencies)
#SignalMonitoring: SignalMonitoring.c RSSI.o shm_write.o shm_read.o Sockets.o
	
#   echo $(Dependencies)

#	gcc -pthread $(Dependencies) SignalMonitoring.c  -o SignalMonitoring -lrt
#	 gcc -pthread SignalMonitoring.c RSSI.o shm_write.o shm_read.o Sockets.o -o SignalMonitoring -lrt
#	touch SignalMonitoring



clean:
	rm -f ClientStart ServerStart SignalMonitoring
	rm *.o
	@echo "clean completed!"
