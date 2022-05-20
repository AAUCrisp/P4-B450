all: Server Client

Server:
	gcc -pthread ServerTest.c -o Server

Client:
	gcc -pthread ClientTest.c -o Client

clean:
	rm -f Server Client
	rm Server Client
	@echo "clean completed!"