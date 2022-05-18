all: ClientStart ServerStart Child

Git:
	git pull

ClientStart: Client.c
	gcc -pthread Client.c -o ClientStart 
	touch ClientTest.c

ServerStart: Parent.cpp shm_write.o shm_read.o
	g++ -pthread Parent.cpp shm_write.o shm_read.o -o ServerStart -lrt
	touch Parent.cpp

Child: child.cpp shm_write.o shm_read.o 
	g++ -pthread child.cpp shm_write.o shm_read.o -o Child -lrt
	touch child.cpp


shm_write.o: Headers/shm_write.c Headers/shm_write_read.h
	g++ -c Headers/shm_write.c -lrt
	touch Headers/shm_write.c 

shm_read.o: Headers/shm_read.c Headers/shm_write_read.h
	g++ -c Headers/shm_read.c -lrt
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
	rm -f ClientStart ServerStart child
	rm *.o
	@echo "clean completed!"