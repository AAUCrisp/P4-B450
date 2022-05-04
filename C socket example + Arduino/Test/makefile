all: NewClientStart NewServerStart

NewClientStart: NewClientTest.c
	gcc -pthread -lconfig NewClientTest.c -o NewClientStart
	touch NewClientStart

NewServerStart: NewServerTest.c
	gcc -pthread -lconfig NewServerTest.c -o NewServerStart
	touch NewServerStart

clean:
	rm -f NewClientStart NewServerStart
	@echo "clean completed!"
