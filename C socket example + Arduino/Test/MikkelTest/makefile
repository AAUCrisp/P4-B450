all: ClientStart ServerStart

ClientStart: TestClientMikkel.c
	gcc -pthread -lconfig TestClientMikkel.c -o ClientStart
	touch ClientStart

ServerStart: TestServerMikkel.c
	gcc -pthread -lconfig TestServerMikkel.c -o ServerStart
	touch ServerStart

clean:
	rm -f ClientStart ServerStart
	@echo "clean completed!"
