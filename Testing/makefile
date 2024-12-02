all: Git Parent Child

Git:
	git pull

Parent: Parent.c shm_read_write.o
	gcc -pthread Parent.c shm_read_write.o -o Parent -lrt
	touch Parent

Child: Child.c shm_read_write.o
	gcc -pthread Child.c shm_read_write.o -o Child -lrt
	touch Child

shm_read_write.o: shm_read_write.c shm_read_write.h 
	gcc -c shm_read_write.c -lrt
	touch shm_read_write.c

clean:
	rm -f Parent Child
	rm *.o 
	@echo "clean completed!"