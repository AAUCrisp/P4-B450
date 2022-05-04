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
#define IP1 "10.20.0.16"
#define IP2 "192.168.1.136"

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

std::string str(LTE);
std::string str2(WiFi);

std::string LTE_interface = "wwan0";
std::string WiFi_interface = "wlan0";

/* Misc */
char message[MAXBUF];
struct sockaddr_in Server_WiFi;
struct sockaddr_in Server_LTE;
int sockLTE, sockWiFi;
int bindLTE, bindWiFi;
unsigned int lenWiFi = sizeof(Server_WiFi), lenLTE = sizeof(Server_LTE);
int f,g;
char ActuatorBuffer[MAXBUF];
int rc_LTE, rc_WiFi;



