
all:  StartClient, StartServer

ClientStart: 
	gcc -pthread -lconfig ClientTest.c -o ClientStart

ServerStart:
	gcc -pthread ServerTest.c -o ServerStart

clean:
	rm -f RunClient StartServer
	@echo "clean completed"

