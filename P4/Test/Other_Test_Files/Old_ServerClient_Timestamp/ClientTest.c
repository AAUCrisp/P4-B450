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
#define MAXBUF 4000
#define PORT1 9000
#define PORT2 9001
const char* IP1 = "10.20.0.16";			// Default Control Unit
const char* IP2 = "192.168.1.136";		// Default Control Unit

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
// char TestMsg1[] = "This is LTE";
// char TestMsg2[] = "This is WiFi";

/* Global signal variable*/
char Temp[MAXBUF];
int GSV;

/*char *Timestamp()
{
	// Timestamp format : [hh:mm:ss dd/mm/yy]
	char curr_time[1];
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(curr_time, "[%d:%d:%d %d/%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

	return curr_time;
}*/

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
/*
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
*/
void *Send_Data_LTE(void *arg)
{
	char *text1 = arg;
	printf("%s\n", text1);
	int test1 = sendto(sockLTE, text1, sizeof(MAXBUF), 0, (struct sockaddr *)&ClientLTE, lenLTE); // send the data to server
	if (test1)
	{
		printf("Message was successfully sent via LTE\n");
	}
	else
	{
		perror("Failed to send via LTE");
	}
}

void *Send_Data_WiFi(void *arg)
{
	char *text2 = arg;
	printf("%s\n", text2);
	int test2 = sendto(sockWiFi, text2, sizeof(MAXBUF), 0, (struct sockaddr *)&ClientWiFi, lenWiFi); // send the data to server
	if (test2)
	{
		printf("Message was successfully sent via WiFi\n");
	}
	else
	{
		perror("Failed to send via WiFi");
	}
}

int main(int argc, char *argv[])
{
	 if(argc > 1) {      // If the program is run with arguments
        printf("\nArgument(s) accepted.\n");

        int aug1 = atoi(argv[1]);

        if(aug1 == 0) {  // For disabling GSV Update
            printf("\n=====  Control Unit IP =====\n");
            IP1 = "10.20.0.16";
			IP2 = "192.168.1.136";
        }
        if(aug1 == 1) {  // For disabling GSV Update
            printf("\n=====  Sensor IP =====\n");
            IP1 = "10.20.0.10";
			IP2 = "192.168.1.160";
        }
        if(aug1 == 2) {  // For disabling GSV Update
            printf("\n=====  Actuator IP =====\n");
            IP1 = "10.20.0.13";
			IP2 = "192.168.1.143";
        }
    }
    else {
        printf("\nNo arguments inserted, using Control Unit IPs.\n");
    }

	/* Create sockets*/
	Create_Socket_LTE();
	Create_Socket_WiFi();

	/* Message to send */
	char TestMsg1[] = "This is LTE something test write this shit";
	char TestMsg2[] = "This is WiFi something test write this shit";
	char newMsg1[] = "SHIT LTE DOES THIS WORK LTE?";
	char newMsg2[] = "FUCK WiFi DOES THIS WORK WiFi?";
	
	//printf("%s\n", newMsg1);
	//printf("%s\n", newMsg2);
	/* Main running code */
	int count = 0;
	while (1)
	{
		//pthread_create(&T1, NULL, Receive_Data_LTE, NULL);
		//pthread_create(&T2, NULL, Receive_Data_WiFi, NULL);
		pthread_create(&T3, NULL, Send_Data_LTE, &newMsg1);
		//pthread_join(T3, NULL);
		pthread_create(&T4, NULL, Send_Data_WiFi, &newMsg2);
		//pthread_join(T4, NULL);
		//int test2 = sendto(sockWiFi, newMsg2, sizeof(MAXBUF), 0, (struct sockaddr *)&ClientWiFi, lenWiFi);

		count++;
		printf("Count is: %d\n", count);
		sleep(5);
		if (count == 50)
		{
			break;
		}
	}
	close(sockLTE && sockWiFi);
	exit(0);
}
