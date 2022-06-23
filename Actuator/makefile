all: Git Actuator ActuatorMonitoring

Git:
	git pull

Actuator: Actuator.o shm_read_write.o ActuatorFunctions.o
	g++ -pthread Actuator.o shm_read_write.o ActuatorFunctions.o -o Actuator -lrt
	touch Actuator

ActuatorMonitoring: ActuatorMonitoring.o shm_read_write.o SocketFunctions.o
	g++ -pthread ActuatorMonitoring.o shm_read_write.o SocketFunctions.o -o ActuatorMonitoring -lrt
	touch ActuatorMonitoring

Actuator.o: Actuator.cpp
	g++ -c -g -pthread Actuator.cpp -lrt
	touch Actuator.cpp

ActuatorMonitoring.o: ActuatorMonitoring.cpp
	g++ -c -g ActuatorMonitoring.cpp -lrt
	touch ActuatorMonitoring.cpp

shm_read_write.o: Headers/shm_read_write.cpp Headers/shm_read_write.h
	g++ -c -g Headers/shm_read_write.cpp -lrt
	touch Headers/shm_read_write.cpp

ActuatorFunctions.o: Headers/ActuatorFunctions.cpp Headers/ActuatorFunctions.h
	g++ -c -g Headers/ActuatorFunctions.cpp -lrt
	touch Headers/ActuatorFunctions.cpp

SocketFunctions.o: Headers/SocketFunctions.cpp Headers/SocketFunctions.h
	g++ -c -g Headers/SocketFunctions.cpp -lrt
	touch Headers/SocketFunctions.cpp

clean:
	rm -f Actuator ActuatorMonitoring
	rm *.o
	rm -f Headers/ActuatorFunctions Headers/shm_read_write Headers/SocketFunctions
	rm *.txt
	@echo "clean completed!"
