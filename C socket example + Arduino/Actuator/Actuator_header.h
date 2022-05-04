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
#define MAXBUF 64
#define PORT 8888
#define PORT2 8887
#define IP1 "10.20.0.16"
#define IP2 "192.168.1.136"

/* Specify LTE / WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

std::string str(LTE);
std::string str2(WiFi);



/* Misc */
char message[MAXBUF];
struct sockaddr_in Client1;
struct sockaddr_in Client2;
int sockfd1, sockfd2;
int len1 = sizeof(Client1), len2 = sizeof(Client2);
int f,g;
