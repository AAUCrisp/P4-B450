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
#define IP "192.168.1.131"
#define IP2 "10.20.0.16"

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
char message[MAXBUF];
struct sockaddr_in Client;
struct sockaddr_in Client2;
int sockfd, len = sizeof(Client);
int sockfd2, len2 = sizeof(Client2);

void *sendshit()
{
	char TestMsg[] = "Client says hello via WiFi!";
	printf("%s\n", TestMsg);
	sendto(sockfd, TestMsg, sizeof(TestMsg), 0, (struct sockaddr *)&Client, len); // send the data to server
	printf("WiFi-Thread id = %ld\n", pthread_self());
	pthread_exit(NULL);
}

void *sendshit2()
{
	char TestMsg[] = "Client says hello via LTE";
	printf("%s\n", TestMsg);
	sendto(sockfd, TestMsg, sizeof(TestMsg), 0, (struct sockaddr *)&Client, len); // send the data to server
	printf("WiFi-Thread id = %ld\n", pthread_self());
	pthread_exit(NULL);
}

int main()
{

	/* Create socket */
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockfd2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));
	setsockopt(sockfd2, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));

	if (sockfd == -1)
	{
		perror("Failed to create socket");
		exit(0);
	}

	/* configure settings to communicate with remote UDP server */
	Client.sin_family = AF_INET;
	Client.sin_port = htons(PORT);
	Client.sin_addr.s_addr = inet_addr(IP);

	Client2.sin_family = AF_INET;
	Client2.sin_port = htons(PORT);
	Client2.sin_addr.s_addr = inet_addr(IP2);

	/* Main running code */
	pthread_t T1, T2;
	while (1)
	{
		sleep(1);
		pthread_create(&T1, NULL, sendshit, NULL);
		pthread_join(T1, NULL);
		pthread_create(&T2, NULL, sendshit2, NULL);
		pthread_join(T2, NULL);
	}
	close(sockfd);
	return 1;
}
