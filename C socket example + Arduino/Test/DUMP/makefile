all: ProcessStart HelloWorld

ProcessStart: ProcessCreationTest.c
	gcc -pthread -lconfig ProcessCreationTest.c -o ProcessStart
	touch ProcessStart

HelloWOrld: HelloWorld.c
	gcc -pthread -lconfig HelloWorld.c -o HelloWorld
	touch HelloWorld

clean:
	rm -f ProcessStart HelloWorld
	@echo "clean completed!"
