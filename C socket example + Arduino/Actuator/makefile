all: ClientStart ServerStart

ClientStart: ActuatorClient.c
	gcc -pthread ActuatorClient.c -o ClientStart
	touch ClientStart
ServerStart: ActuatorServer.c
	g++ -pthread  ActuatorServer.c -o ServerStart
	touch ServerStart
clean:
	rm -f ClientStart ServerStart
	@echo "clean completed!"
