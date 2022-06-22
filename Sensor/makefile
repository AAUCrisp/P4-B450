#all: Sensor SensorMonitoring
all: Git Sensor SensorMonitoring

Git:
	git pull

Sensor: Sensor.c SocketFunctions.o shm_read_write.o
	gcc -pthread Sensor.c SocketFunctions.o shm_read_write.o -o Sensor -lrt
	touch Sensor

SensorMonitoring: SensorMonitoring.c shm_read_write.o SocketFunctions.o
	gcc -pthread SensorMonitoring.c shm_read_write.o SocketFunctions.o -o SensorMonitoring -lrt
	touch SensorMonitoring

SocketFunctions.o: Headers/SocketFunctions.c Headers/SocketFunctions.h Headers/shm_read_write.h
	gcc -c Headers/SocketFunctions.c -lrt
	touch Headers/SocketFunctions

shm_read_write.o: Headers/shm_read_write.c Headers/shm_read_write.h
	gcc -c Headers/shm_read_write.c -lrt
	touch Headers/shm_read_write
	
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
