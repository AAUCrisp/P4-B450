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
#define PORT1 8888
#define PORT2 8887
#define IP1 "10.20.0.16"
#define IP2 "192.168.1.136"

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
char message[MAXBUF];
struct sockaddr_in ClientLTE;
struct sockaddr_in ClientWiFi;
int sockLTE, sockWiFi;
int lenLTE = sizeof(ClientLTE);
int lenWiFi = sizeof(ClientWiFi);
int rc_LTE, rc_WiFi;
pthread_t T1, T2, T3, T4;

/* Message to send */
char TestMsg1[] = "This is LTE";
char TestMsg2[] = "This is WiFi";

/* Global signal variable*/
char Temp[MAXBUF];
int GSV;

void Create_Socket_LTE()
{
	/* Create socket */
	sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));

	if (sockLTE == -1)
	{
		perror("Failed to create socket: LTE");
		exit(0);
	}
	else
	{
		printf("LTE socket was created successfully\n");
	}

	/* configure settings to communicate with remote UDP server */
	ClientLTE.sin_family = AF_INET;
	ClientLTE.sin_port = htons(PORT1);
	ClientLTE.sin_addr.s_addr = inet_addr(IP1);
}

void Create_Socket_WiFi()
{
	/* Create socket */
	sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

	if (sockWiFi == -1)
	{
		perror("Failed to create socket: WiFi");
		exit(0);
	}
	else
	{
		printf("WiFi socket was successful! \n");
	}

	/* configure settings to communicate with remote UDP server */
	ClientWiFi.sin_family = AF_INET;
	ClientWiFi.sin_port = htons(PORT2);
	ClientWiFi.sin_addr.s_addr = inet_addr(IP2);
}

void *Receive_Data_LTE()
{
	rc_LTE = recvfrom(sockLTE, Temp, MAXBUF, 0, (struct sockaddr *)&ClientLTE, &lenLTE);
	printf("LTE-Thread id = %ld\n", pthread_self());
	if (rc_LTE == -1)
	{
		perror("Failed to receive LTE msg");
	}
	else
	{
		printf("%s\n", Temp);
		pthread_exit(NULL);
	}
}

void *Receive_Data_WiFi()
{
	rc_WiFi = recvfrom(sockWiFi, Temp, MAXBUF, 0, (struct sockaddr *)&ClientWiFi, &lenWiFi);
	printf("WiFi-Thread id = %ld\n", pthread_self());
	if (rc_WiFi == -1)
	{
		perror("Failed to receive WiFi msg");
	}
	else
	{
		printf("%s\n", Temp);
		pthread_exit(NULL);
	}
}

void *Send_Data_LTE(void *arg)
{
	int test1 = sendto(sockLTE, arg, sizeof(arg), 0, (struct sockaddr *)&ClientLTE, lenLTE); // send the data to server
	if (test1 == sizeof(arg))
	{
		printf("%p was successfully sent!\n", arg);
	}
}

void *Send_Data_WiFi(void *arg)
{
	int test2 = sendto(sockWiFi, arg, sizeof(arg), 0, (struct sockaddr *)&ClientWiFi, lenWiFi); // send the data to server
	if (test2 == sizeof(arg))
	{
		printf("%p was successfully sent!\n", arg);
	}
}

int main()
{
	Create_Socket_LTE();
	Create_Socket_WiFi();

	/* Main running code */
	int count = 0;
	while (1)
	{
		pthread_create(&T1, NULL, Receive_Data_LTE, NULL);
		pthread_create(&T2, NULL, Receive_Data_WiFi, NULL);
		pthread_create(&T3, NULL, Send_Data_LTE, &TestMsg1);
		pthread_create(&T4, NULL, Send_Data_WiFi, &TestMsg2);

		count++;
		printf("Count is: %d\n", count);
		sleep(1);
		if (count == 10)
		{
			break;
		}
	}
	close(sockLTE && sockWiFi);
	exit(0);
}
