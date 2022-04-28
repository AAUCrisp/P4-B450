
#all:  ClientStart ServerStart
	

#ClientStart: 
#	gcc -pthread -lconfig ClientTest.c -o ClientStart
#	touch ClientStart

ServerStart: ServerTest.c
	gcc -pthread ServerTest.c -o ServerStart
	touch ServerStart

clean:
	rm -f ServerStart
	@echo "clean completed!"
