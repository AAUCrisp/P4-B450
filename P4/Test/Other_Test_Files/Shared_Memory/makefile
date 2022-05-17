all: shm_server shm_client

shm_server: shm_server.c
	gcc -pthread -lconfig shm_server.c -o shm_server
	touch shm_server

shm_client: shm_client.c
	gcc -pthread -lconfig shm_client.c -o shm_client
	touch shm_client

clean:
	rm -f shm_server shm_client
	@echo "clean completed!"
