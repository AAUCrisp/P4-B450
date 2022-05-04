#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

/* Define buffer size, PORT number and server IP */
#define MAXBUF 64
#define PORT 6000
#define IP "127.0.0.1"

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "lo";

/* Misc */
char message[MAXBUF];
struct sockaddr_in Client;
int sockfd, len = sizeof(Client);

void* sendshit(){
	char TestMsg[] = "Hello does this work? I am testing if it is actually sending this shit to the server via threads";
	printf("%s\n", TestMsg);
	sendto(sockfd, TestMsg, sizeof(TestMsg), 0, (struct sockaddr *)&Client, len); // send the data to server
	printf("WiFi-Thread id = %ld\n", pthread_self());
	pthread_exit(NULL);
}

int main()
{

	/* Create socket */
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

	if (sockfd == -1)
	{
		perror("Failed to create socket");
		exit(0);
	}

	/* configure settings to communicate with remote UDP server */
	Client.sin_family = AF_INET;
	Client.sin_port = htons(PORT);
	Client.sin_addr.s_addr = inet_addr(IP);

	/* Main running code */
	pthread_t T1;
	while (1)
	{
		sleep(1);
		pthread_create(&T1, NULL, sendshit, NULL);
		
	}
	close(sockfd);
	return 1;
}
