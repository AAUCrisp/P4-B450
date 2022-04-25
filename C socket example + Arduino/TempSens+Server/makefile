
all: RunClient StartServer

RunClient: ClientTemp.o  TempSensorStub.o 
	gcc -pthread `pkg-config --cflags libconfig` ClientTemp.o \
    `pkg-config --libs libconfig` TempSensorStub.o -o RunClient

StartServer: ServerTest2.o
	gcc -pthread ServerTest2.o -o StartServer

ClientTemp.o: ClientTemp.c 
	gcc -c ClientTemp.c 

TempSensorStub.o: TempSensorStub.c TempSensorStub.h
	gcc -c TempSensorStub.c 

Server.o: ServerTest2.c
	gcc -c -pthread ServerTest2.c
	
clean:
	rm *.o RunClient StartServer
	@echo "clean completed"

