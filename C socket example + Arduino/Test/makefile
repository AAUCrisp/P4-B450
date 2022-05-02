all: ClientStart ServerStart

ClientStart: ClientTest.c
	gcc -pthread -lconfig ClientTest.c -o ClientStart
	touch ClientStart

ServerStart: ServerTest.c
	gcc -pthead -lconfig ServerTest.c -o ServerStart
	touch ServerStart

clean:
	rm -f ClientStart ServerStart
	@echo "clean completed!"
