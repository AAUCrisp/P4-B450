#all: Sensor SensorMonitoring
all: Git Sensor SensorMonitoring

Git:
	git pull

Sensor: Sensor.c SocketFunctions.o shm_write.o shm_read.o
	gcc -pthread Sensor.c SocketFunctions.o shm_write.o shm_read.o -o Sensor -lrt
	touch Sensor

SensorMonitoring: SensorMonitoring.c shm_write.o shm_read.o SocketFunctions.o
	gcc -pthread SensorMonitoring.c shm_write.o shm_read.o SocketFunctions.o -o SensorMonitoring -lrt
	touch SensorMonitoring

SocketFunctions.o: Headers/SocketFunctions.c Headers/SocketFunctions.h Headers/shm_write_read.h
	gcc -c Headers/SocketFunctions.c -lrt
	touch Headers/SocketFunctions.c

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

clean:
	rm -f Sensor SensorMonitoring
	rm *.o
#	rm *.txt
	@echo "clean completed!"
