all: ProcessStart HelloWorld

ProcessStart: ProcessCreationTest.c shm_write.o
	gcc -pthread -lconfig ProcessCreationTest.c shm_write.o -o ProcessStart
	touch ProcessStart

HelloWorld: HelloWorld.c shm_read.o
	gcc -pthread -lconfig HelloWorld.c shm_read.o -o HelloWorld
	touch HelloWorld

shm_write.o: shm_write.c shm_write_read.h
	gcc -c shm_write.c 

shm_read.o: shm_read.c shm_write_read.h
	gcc -c shm_read.c 

clean:
	rm -f ProcessStart HelloWorld
	rm *.o 
	@echo "clean completed!"
