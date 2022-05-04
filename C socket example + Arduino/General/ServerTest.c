#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

/* Define buffer and PORT number */
#define MAXBUF 1024
#define PORT 6000

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
struct sockaddr_in Server;
struct sockaddr_in Server2;
int sockfd, len = sizeof(Server);
int sockfd2, len2 = sizeof(Server2);
char Buffer[MAXBUF];
char Buffer2[MAXBUF];

void *receiveshit()
{
	int rc = recvfrom(sockfd, Buffer, MAXBUF, 0, (struct sockaddr *)&Server, &len);
	printf("WiFi-Thread id = %ld\n", pthread_self());
	printf("%s\n \n", Buffer);
}

void *receiveshit2()
{
	int rc2 = recvfrom(sockfd2, Buffer2, MAXBUF, 0, (struct sockaddr *)&Server2, &len2);
	printf("LTE-Thread id = %ld\n", pthread_self());
	printf("%s\n \n", Buffer2);
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

	/* Configure settings to communicate with remote UDP server */
	Server.sin_family = AF_INET;
	Server.sin_port = htons(PORT);
	Server.sin_addr.s_addr = INADDR_ANY;

	Server2.sin_family = AF_INET;
	Server2.sin_port = htons(PORT);
	Server2.sin_addr.s_addr = INADDR_ANY;

	/* Bind to socket */
	int b = bind(sockfd, (struct sockaddr *)&Server, sizeof(struct sockaddr));
	int b2 = bind(sockfd2, (struct sockaddr *)&Server2, sizeof(struct sockaddr));
	if (b == -1)
	{
		perror("Failed to bind");
		close(sockfd);
		exit(0);
	}

	/* Main running code */
	pthread_t T1, T2;
	while (1)
	{
		// puts("Emergency exit: CTRL+C");
		// printf("Waiting for data...\n");
		sleep(1);
		pthread_create(&T1, NULL, receiveshit, NULL);
		pthread_join(T1, NULL);
		pthread_create(&T1, NULL, receiveshit2, NULL);
		pthread_join(T2, NULL);
		/*int rc = recvfrom(sockfd, Buffer, MAXBUF, 0, (struct sockaddr *)&Server, &len);
		printf("%s\n \n", Buffer);*/
	}
	close(sockfd);
	return 1;
}
