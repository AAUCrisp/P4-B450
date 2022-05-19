all: Git Actuator ActuatorMonitoring

Git:
	git pull

Actuator: Actuator.cpp SocketFunctions.o shm_write.o shm_read.o
	g++ -pthread Actuator.cpp SocketFunctions.o shm_write.o shm_read.o -o Actuator -lrt
	touch Actuator

ActuatorMonitoring: ActuatorMonitoring.c shm_write.o shm_read.o SocketFunctions.o
	g++ -pthread ActuatorMonitoring.c shm_write.o shm_read SocketFunctions.o
	touch ActuatorMonitoring

ActuatorFunctions.o: Headers/ActuatorFunctions.c Headers/ActuatorFunctions.h 
	g++ Headers/ActuatorFunctions.c -lrt
	touch Headers/ActuatorFunctions.c

SocketFunctions.o: Headers/SocketFunctions.c Headers/SocketFunctions.h Headers/shm_write_read.h
	gcc -c Headers/SocketFunctions.c -lrt
	touch Headers/SocketFunctions.c

shm_write.o: Headers/shm_write.c Headers/shm_write_read.h
	gcc -c Headers/shm_write.c -lrt
	touch Headers/shm_write.c 

shm_read.o: Headers/shm_read.c Headers/shm_write_read.h
	gcc -c Headers/shm_read.c -lrt
	touch Headers/shm_read.c 

clean:
	rm -f Sensor SensorMonitoring
	rm *.o
	@echo "clean completed!"
