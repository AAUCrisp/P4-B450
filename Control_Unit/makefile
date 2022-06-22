all: git SignalMonitoring ControlUnit

git:
	git pull

#NYT FIX: HUSK OG FIX HEADERS OG INCLUDES...
Control_Unit: ControlUnit.o Sockets.o shm_read_write.o
	g++ -pthread Control_Unit.o Sockets.o shm_read_write.o -o ControlUnit -lrt

#NYT FIX: HUSK OG FIX HEADERS OG INCLUDES...
SignalMonitoring: SignalMonitoring.o Sockets.o RSSI.o shm_read_write.o
	g++ -pthread SignalMonitoring.o Sockets.o RSSI.o shm_read_write.o -o SignalMonitoring -lrt

ControlUnit.o: Control_Unit.cpp
	g++ -c -g -o -pthread Control_Unit.cpp
	touch Control_Unit

SignalMonitoring.o: SignalMonitoring.cpp
	g++ -c -g -pthread SignalMonitoring.cpp -lrt
	touch SignalMonitoring

Sockets.o: Headers/Sockets.cpp Headers/Sockets.h
	g++ -c Headers/Sockets.cpp -lrt
	touch Headers/Sockets

RSSI.o: Headers/RSSI.cpp Headers/Sockets.h
	g++ -c Headers/RSSI.cpp -lrt
	touch Headers/RSSI

shm_read_write.o: Headers/shm_read_write.cpp Headers/shm_read_write.h
	g++ -c -g Headers/shm_read_write.cpp -lrt
	touch Headers/shm_read_write


# Dependencies 
#Dependencies = $(wildcard */*.cpp)

#test: Libraries.cpp $(Dependencies)
#	g++ -c Libraries.cpp $(Dependencies) -lrt
#	touch Libraries.cpp


clean:
	rm -f ControlUnit SignalMonitoring
	rm *.o
	rm -f Headers/RSSI Headers/shm_read_write Headers/Sockets
	@echo "clean completed!"