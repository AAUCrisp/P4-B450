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
#define SENSBUF 1024
#define ACTBUF 1024
#define PORT1 8888
#define PORT2 8887

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan1";

/* Misc */
struct sockaddr_in ServerLTE;
struct sockaddr_in ServerWiFi;
int sockLTE, sockWiFi;
int bindLTE, bindWiFi;
int lenLTE = sizeof(ServerLTE);
int lenWiFi = sizeof(ServerWiFi);
char SensorBuffer[SENSBUF];
char ActuatorBuffer[ACTBUF];
int rc_LTE, rc_WiFi;
pthread_t T1, T2;

void Create_Bind_Socket_LTE()
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

	/* Configure settings to communicate with remote UDP server */
	ServerLTE.sin_family = AF_INET;
	ServerLTE.sin_port = htons(PORT1);
	ServerLTE.sin_addr.s_addr = INADDR_ANY;

	/* Bind to socket */
	bindLTE = bind(sockLTE, (struct sockaddr *)&ServerLTE, sizeof(struct sockaddr));

	if (bindLTE == -1)
	{
		perror("Failed to bind: LTE");
		exit(0);
	}
	else
	{
		printf("LTE bind was successful! \n");
	}
}

void Create_Bind_Socket_WiFi()
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

	/* Configure settings to communicate with remote UDP server */
	ServerWiFi.sin_family = AF_INET;
	ServerWiFi.sin_port = htons(PORT2);
	ServerWiFi.sin_addr.s_addr = INADDR_ANY;

	/* Bind to socket */
	bindWiFi = bind(sockWiFi, (struct sockaddr *)&ServerWiFi, sizeof(struct sockaddr));

	if (bindWiFi == -1)
	{
		perror("Failed to bind: WiFi");
		exit(0);
	}
	else
	{
		printf("WiFi bind was successful! \n");
	}
}

void *Receive_Data_LTE()
{
	rc_LTE = recvfrom(sockLTE, SensorBuffer, SENSBUF, 0, (struct sockaddr *)&ServerLTE, &lenLTE);
	printf("%s\n \n", SensorBuffer);
	pthread_exit(0);
}

void *Receive_Data_WiFi()
{
	rc_WiFi = recvfrom(sockWiFi, ActuatorBuffer, ACTBUF, 0, (struct sockaddr *)&ServerWiFi, &lenWiFi);
	printf("%s\n \n", ActuatorBuffer);
	pthread_exit(0);
}

/* No need to run sudo when running server*/
int main()
{
	/* Binding sockets to LTE/WiFi */
	Create_Bind_Socket_LTE();
	Create_Bind_Socket_WiFi();

	/* Main running code */
	while (1)
	{
		/* Creating threads running receive data functions */
		pthread_create(&T1, NULL, Receive_Data_LTE, NULL);
		pthread_create(&T2, NULL, Receive_Data_WiFi, NULL);
		usleep(100);
		//pthread_join(T1, NULL);
	        //pthread_join(T2, NULL);
		usleep(100);
		// Receive_Data_LTE();
		// Receive_Data_WiFi();
	}
	close(sockLTE && sockWiFi);
	return 1;
}
