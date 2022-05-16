all: ClientStart ServerStart

ClientStart: ActuatorClient.c
	gcc -pthread ActuatorClient.c -o ClientStart
	touch ClientStart
ServerStart: ActuatorServer.cpp
	g++ -pthread  ActuatorServer.cpp -o ServerStart
	touch ServerStart
clean:
	rm -f ClientStart ServerStart
	@echo "clean completed!"
