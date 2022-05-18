all: ControlUnit SignalMonitoring

Git:
	git pull

Combined: combined.cpp

ControlUnit: Control_Unit.cpp Sockets.o RSSI.o shm_write.o shm_read.o
	g++ -pthread Control_Unit.cpp Sockets.o RSSI.o -o ControlUnit -lrt
	touch Control_Unit.cpp

SignalMonitoring: SignalMonitoring.cpp Sockets.o RSSI.o shm_write.o shm_read.o
	g++ -pthread SignalMonitoring.cpp Sockets.o RSSI.o -o SignalMonitoring -lrt
	touch SignalMonitoring.cpp

Sockets.o: Headers/Sockets.cpp Headers/Sockets.h Headers/shm_write_read.h
	g++ -c Headers/Sockets.cpp -lrt
	touch Headers/Sockets.cpp

RSSI.o: Headers/RSSI.cpp Headers/Sockets.h
	g++ -c Headers/RSSI.cpp -lrt
	touch Headers/RSSI.cpp

shm_write.o: Headers/shm_write.cpp
	g++ -c Headers/shm_write.cpp -lrt
	touch Headers/shm_write.cpp 

shm_read.o: Headers/shm_read.cpp
	g++ -c Headers/shm_read.cpp -lrt
	touch Headers/shm_read.cpp 

SMH.o: Headers/shm_write_read.h Headers/shm_write.cpp Headers/shm_read.cpp
	g++ -c Headers/shm_write.cpp Headers/shm_read.cpp
	touch Headers/shm_write.cpp


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
	@echo "clean completed!"
