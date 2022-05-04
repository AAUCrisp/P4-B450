all: NewClientStart ServerStart

NewClientStart: NewClientTest.c
	gcc -pthread -lconfig NewClientTest.c -o NewClientStart
	touch NewClientStart

ServerStart: ServerTest.c
	gcc -pthread -lconfig ServerTest.c -o ServerStart
	touch ServerStart

clean:
	rm -f NewClientStart ServerStart
	@echo "clean completed!"
