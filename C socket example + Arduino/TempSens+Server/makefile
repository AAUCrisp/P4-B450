
all:  StartServer

#RunClient: ClientTemp.o  TempSensorStub.o 
#	gcc -pthread -lconfig ClientTemp.o TempSensorStub.o -o RunClient 

StartServer: ServerTest2.o
	gcc -pthread ServerTest2.o -o StartServer

#ClientTemp.o: ClientTemp.c 
#	gcc -c -lconfig  ClientTemp.c

#TempSensorStub.o: TempSensorStub.c TempSensorStub.h
#	gcc -c -lconfig TempSensorStub.c 

Server.o: ServerTest2.c
	gcc -c -pthread -lconfig ServerTest2.c
	
clean:
	rm *.o RunClient StartServer
	@echo "clean completed"

