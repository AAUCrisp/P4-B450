all: Git Process Child

Git:
	git pull

Process: Process.c shm_read.o
	gcc -pthread Process.c shm_read.o -o Process -lrt
	touch Process

Child: Child.c shm_write.o
	gcc -pthread Child.c shm_write.o -o Child -lrt
	touch Child

shm_write.o: shm_write.c shm_read_write.h 
	gcc -c shm_write.c -lrt
	touch shm_write.c 

shm_read.o: shm_read.c shm_read_write.h 
	touch shm_read.c 

clean:
	rm -f Process Child
	rm *.o 
	@echo "clean completed!"