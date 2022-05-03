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
#define PORT 8888
#define PORT2 8887
#define IP1 "10.20.0.16"
#define IP2 "192.168.1.136"

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
char message[MAXBUF];
struct sockaddr_in Client1;
struct sockaddr_in Client2;
int sockfd1, sockfd2;
int len1 = sizeof(Client1), len2 = sizeof(Client2);

int main()
{

	/* Create socket */
	sockfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockfd2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sockfd1, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
	setsockopt(sockfd2, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

	if (sockfd1 == -1)
	{
		perror("Failed to create socket");
		exit(0);
	}

	/* configure settings to communicate with remote UDP server */
	Client1.sin_family = AF_INET;
	Client1.sin_port = htons(PORT);
	Client1.sin_addr.s_addr = inet_addr(IP1);

	Client2.sin_family = AF_INET;
	Client2.sin_port = htons(PORT2);
	Client2.sin_addr.s_addr = inet_addr(IP2);

	/* Main running code */
	int count = 0;
	while (1)
	{
		char TestMsg1[] = "This is LTE";
		char TestMsg2[] = "This is WiFi";
		int test1 = sendto(sockfd1, TestMsg1, sizeof(TestMsg1), 0, (struct sockaddr *)&Client1, len1); // send the data to server
		if (test1 == sizeof(TestMsg1)){
			printf("TestMsg1 was successfully sent!\n");
		}
		int test2 = sendto(sockfd2, TestMsg2, sizeof(TestMsg2), 0, (struct sockaddr *)&Client2, len2); // send the data to server
		if (test2 == sizeof(TestMsg2)){
			printf("TestMsg2 was successfully sent!\n");
		}
		count++;
		printf("Count is: %d\n", count);
		sleep(1);
		if(count == 10){
		close(sockfd1);
		close(sockfd2);
		exit(0);
		}
	}
	close(sockfd1 && sockfd2);
	return 1;
}
