
all:  ClientStart ServerStart
	

ClientStart: 
	gcc -pthread -lconfig ClientTest.c -o ClientStart
	touch ClientStart

ServerStart:
	gcc -pthread ServerTest.c -o ServerStart
	touch ServerStart

clean:
	rm -f ClientStart ServerStart
	@echo "clean completed!"
