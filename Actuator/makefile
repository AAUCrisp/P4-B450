all: Actuator ActuatorMonitoring

git:
	git pull
	make
	rm Logs/log.txt Logs/processed_commands.txt

Actuator: Actuator.o shm_read_write.o ActuatorFunctions.o
	g++ -pthread Actuator.o shm_read_write.o ActuatorFunctions.o -o Actuator -lrt

ActuatorMonitoring: ActuatorMonitoring.o shm_read_write.o SocketFunctions.o
	g++ -pthread ActuatorMonitoring.o shm_read_write.o SocketFunctions.o -o ActuatorMonitoring -lrt

Actuator.o: Actuator.cpp
	g++ -c -g -pthread Actuator.cpp -lrt
#	touch Actuator

ActuatorMonitoring.o: ActuatorMonitoring.cpp
	g++ -c -g ActuatorMonitoring.cpp -lrt
#	touch ActuatorMonitoring

shm_read_write.o: Headers/shm_read_write.cpp Headers/shm_read_write.h
	g++ -c -g Headers/shm_read_write.cpp -lrt
#	touch Headers/shm_read_write

ActuatorFunctions.o: Headers/ActuatorFunctions.cpp Headers/ActuatorFunctions.h
	g++ -c -g Headers/ActuatorFunctions.cpp -lrt
#	touch Headers/ActuatorFunctions

SocketFunctions.o: Headers/SocketFunctions.cpp Headers/SocketFunctions.h
	g++ -c -g Headers/SocketFunctions.cpp -lrt
#	touch Headers/SocketFunctions

clean:
	rm -f Actuator ActuatorMonitoring
	rm *.o
#	rm -f Headers/ActuatorFunctions Headers/shm_read_write Headers/SocketFunctions
#	rm *.txt
	@echo "clean completed!"
