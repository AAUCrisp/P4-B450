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
#define PORT1 8888
#define PORT2 8887

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
struct sockaddr_in ServerLTE;
struct sockaddr_in ServerWiFi;
int sockLTE, sockWiFi;
int bindLTE, bindWiFi;
int lenLTE = sizeof(ServerLTE);
int lenWiFi = sizeof(ServerWiFi);
char SensorBuffer[MAXBUF];
char ActuatorBuffer[MAXBUF];
int rc_LTE, rc_WiFi;
pthread_t T1, T2;

void *Receive_Data_LTE()
{
	rc_LTE = recvfrom(sockLTE, SensorBuffer, sizeof(SensorBuffer), 0, (struct sockaddr *)&ServerLTE, &lenLTE);
	printf("LTE-Thread id = %ld\n", pthread_self());
	if (rc_LTE == -1)
	{
		perror("Failed to receive LTE msg");
	}
	else
	{
		printf("%s\n", SensorBuffer);
		//sleep(1);
		//pthread_exit(NULL);
	}
}

void *Receive_Data_WiFi()
{
	rc_WiFi = recvfrom(sockWiFi, ActuatorBuffer, sizeof(ActuatorBuffer), 0, (struct sockaddr *)&ServerWiFi, &lenWiFi);
	printf("WiFi-Thread id = %ld\n", pthread_self());
	if (rc_WiFi == -1)
	{
		perror("Failed to receive WiFi msg");
	}
	else
	{
		printf("%s\n", ActuatorBuffer);
		//sleep(1);
		//pthread_exit(NULL);
	}
}
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

int main() {
  Create_Bind_Socket_LTE();
  Create_Bind_Socket_WiFi();
  /* Main running code */
  while (1){

    pthread_create(&T1, NULL, Receive_Data_LTE, NULL);
    pthread_create(&T2, NULL, Receive_Data_WiFi, NULL);
    pthread_join(T1, NULL);
    free(T1);
    pthread_join(T2, NULL);
    free(T2);
    sleep(1);
    count++;
		printf("Count: %d\n", count);
		if (count == 50)
		{
			break;
		}
  }
  close(sockLTE && sockWiFi);
  return 1;
}
