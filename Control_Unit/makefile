all: git SignalMonitoring ControlUnit

git:
	git pull


#ControlUnit: Control_Unit.cpp Sockets.o shm_read.o 
#	g++ -pthread Control_Unit.cpp Sockets.o -o ControlUnit -lrt

#SignalMonitoring: SignalMonitoring.cpp Sockets.o RSSI.o shm_write.o shm_read.o
#	g++ -pthread SignalMonitoring.cpp Sockets.o RSSI.o -o SignalMonitoring -lrt
#	touch SignalMonitoring
Control_Unit: ControlUnit.o Sockets.o shm_read.o
	g++ -pthread Control_Unit.o Sockets.o -o ControlUnit -lrt

SignalMonitoring: SignalMonitoring.o Sockets.o RSSI.o shm_write.o shm_read.o
	g++ -pthread SignalMonitoring.o Sockets.o RSSI.o shm_write.o shm_read.o -o SignalMonitoring -lrt

ControlUnit.o: Control_Unit.cpp
	g++ -c -g -o -pthread Control_Unit.cpp
	touch Control_Unit

SignalMonitoring.o: SignalMonitoring.cpp
	g++ -c -g -pthread SignalMonitoring.cpp -lrt
	touch SignalMonitoring

Sockets.o: Headers/Sockets.cpp Headers/Sockets.h shm_read.o shm_write.o
	g++ -c Headers/Sockets.cpp -lrt
	touch Headers/Sockets

RSSI.o: Headers/RSSI.cpp Headers/Sockets.h
	g++ -c Headers/RSSI.cpp -lrt
	touch Headers/RSSI

shm_write.o: Headers/shm_write.cpp
	g++ -c Headers/shm_write.cpp -lrt
	touch Headers/shm_write

shm_read.o: Headers/shm_read.cpp
	g++ -c Headers/shm_read.cpp -lrt
	touch Headers/shm_read

shm.o: Headers/shm_write.cpp Headers/shm_read.cpp Headers/shm_write_read.h 
	g++ -c Headers/shm_write.cpp Headers/shm_read.cpp
	touch Headers/shm_write Headers/shm_read


# Dependencies 
Dependencies = $(wildcard */*.cpp)

test: Libraries.cpp $(Dependencies)
	g++ -c Libraries.cpp $(Dependencies) -lrt
	touch Libraries.cpp


# TESTING SHIT!
# SignalMonitoring: SignalMonitoring.cpp $(Dependencies)
#SignalMonitoring: SignalMonitoring.cpp RSSI.o shm_write.o shm_read.o Sockets.o
	
#   echo $(Dependencies)

#	g++ -pthread $(Dependencies) SignalMonitoring.cpp  -o SignalMonitoring -lrt
#	 g++ -pthread SignalMonitoring.cpp RSSI.o shm_write.o shm_read.o Sockets.o -o SignalMonitoring -lrt
#	touch SignalMonitoring



clean:
	rm -f ControlUnit SignalMonitoring
	rm *.o
	rm -f Headers/RSSI Headers/shm_read Headers/shm_write Headers/Sockets
	@echo "clean completed!"
