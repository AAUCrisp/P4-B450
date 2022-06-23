all: SignalMonitoring ControlUnit

git:
	git pull
	make

#NYT FIX: HUSK OG FIX HEADERS OG INCLUDES...
ControlUnit: ControlUnit.o Sockets.o shm_read_write.o
	g++ -pthread ControlUnit.o Sockets.o shm_read_write.o -o ControlUnit -lrt

#NYT FIX: HUSK OG FIX HEADERS OG INCLUDES...
SignalMonitoring: SignalMonitoring.o Sockets.o RSSI.o shm_read_write.o Functions.o
	g++ -pthread SignalMonitoring.o Sockets.o RSSI.o shm_read_write.o Functions.o -o SignalMonitoring -lrt

ControlUnit.o: ControlUnit.cpp
	g++ -c -g  -pthread ControlUnit.cpp -lrt
	touch ControlUnit

SignalMonitoring.o: SignalMonitoring.cpp
	g++ -c -g -pthread SignalMonitoring.cpp -lrt
	touch SignalMonitoring

Sockets.o: Headers/Sockets.cpp Headers/Sockets.h
	g++ -c Headers/Sockets.cpp -lrt
	touch Headers/Sockets

RSSI.o: Headers/RSSI.cpp Headers/Sockets.h
	g++ -c Headers/RSSI.cpp -lrt
	touch Headers/RSSI

shm_read_write.o: Headers/shm_read_write.c Headers/shm_read_write.h
	g++ -c -g Headers/shm_read_write.c -lrt
	touch Headers/shm_read_write

Functions.o: Headers/Functions.cpp
	g++ -c -g Headers/Functions.cpp -lrt
	touch Headers/Functions


# Dependencies 
#Dependencies = $(wildcard */*.cpp)

#test: Libraries.cpp $(Dependencies)
#	g++ -c Libraries.cpp $(Dependencies) -lrt
#	touch Libraries.cpp


clean:
	rm -f ControlUnit SignalMonitoring
	rm *.o
	rm -f Headers/RSSI Headers/shm_read_write Headers/Sockets Headers/Functions
	rm *.txt
	@echo "clean completed!"