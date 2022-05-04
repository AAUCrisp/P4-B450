#include <stdio.h>
//#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string>

/* Define buffer size, PORT number and server IP */
#define MAXBUF 1024
#define PORT 8888
#define PORT2 8887
#define IP1 "10.20.0.13"
#define IP2 "192.168.1.143"

/* Specify LTE / WiFi interface */
//const char *LTE = "wwan0";
//const char *WiFi = "wlan0";

//std::string str(LTE);
//std::string str2(WiFi);

std::string LTE_interface = "wwan0";
const char *LTE = LTE_interface.c_str();
std::string WiFi_interface = "wlan0";
const char *WiFi = WiFi_interface.c_str();


/* Misc */
char message[MAXBUF];
struct sockaddr_in Server_WiFi;
struct sockaddr_in Server_LTE;
int sockLTE, sockWiFi;
int bindLTE, bindWiFi;
unsigned int lenWiFi = sizeof(Server_WiFi), lenLTE = sizeof(Server_LTE);
int f,g,i;
char ActuatorBuffer[MAXBUF];
int rc_LTE, rc_WiFi;

int socket_configuration() {
       /* configure settings to communicate with remote UDP server */
	Server_LTE.sin_family = AF_INET;
	Server_LTE.sin_port = htons(PORT);
	Server_LTE.sin_addr.s_addr = inet_addr(IP1);
	Server_WiFi.sin_family = AF_INET;
	Server_WiFi.sin_port = htons(PORT2);
	Server_WiFi.sin_addr.s_addr = inet_addr(IP2);


	/* Create socket and bind them to Interface*/
	sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	f = setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, LTE_interface.length());
	g = setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, WiFi_interface.length());

      /* Error Handling of socket creation, and interface binding*/
	if (sockLTE == -1)
	{
		perror("Failed to create socket");
		exit(0);
	}

	if (f == -1) 
	{
		perror("Error with LTE...the error is");
		return(-1);
		exit(0);
	} 

	if (g == -1)

	{
		perror("Error with WiFi... the error is");
		return(-1);
		exit(0);
	}

	
    bindLTE = bind(sockLTE, (struct sockaddr *)&Server_LTE, sizeof(struct sockaddr));
    bindWiFi = bind(sockWiFi, (struct sockaddr *)&Server_WiFi, sizeof(struct sockaddr));
    
    return 0;

}


