all: Actuator ActuatorMonitoring


Actuator: Actuator.cpp 
	g++ -pthread Actuator.cpp -o Actuator -lrt
	touch Actuator

ActuatorMonitoring: ActuatorMonitoring.c  SocketFunctions.o
	g++ -pthread ActuatorMonitoring.c  SocketFunctions.o -o ActuatorMonitoring -lrt
	touch ActuatorMonitoring

SocketFunctions.o: Headers/SocketFunctions.c Headers/SocketFunctions.h Headers/shm_write_read.h
	g++ -c Headers/SocketFunctions.c -lrt
	touch Headers/SocketFunctions.c

shm_write.o: Headers/shm_write.c Headers/shm_write_read.h
	g++ -c Headers/shm_write.c -lrt
	touch Headers/shm_write.c 

shm_read.o: Headers/shm_read.c Headers/shm_write_read.h
	g++ -c Headers/shm_read.c -lrt
	touch Headers/shm_read.c 

clean:
	rm -f Actuator ActuatorMonitoring
	rm *.o
	rm *.txt
	@echo "clean completed!"
