#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>  //IPC thing
#include <sys/mman.h>
#include <sys/shm.h>  //SHM thing
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <netdb.h>

struct addrinfo ServerLTE, *res1;   // a socket struct design to be used with IPv4
struct addrinfo ServerWiFi, *res;  // a socket struct design to be used with IPv4
struct sockaddr_storage remote_addr;
socklen_t fromlen;
int rv, rl;
int sockLTE, sockWiFi;
int bindLTE, bindWiFi;
int lenLTE = sizeof(ServerLTE);
int lenWiFi = sizeof(ServerWiFi);
int rc_LTE, rc_WiFi;
int tx_LTE, tx_WiFI;
pthread_t T1, T2;

char *received_message;
const char* GSV_KEY = "GSV_KEY";

//std::ofstream LTE_File;
//std::ofstream WiFi_File;


char ActuatorBuffer[1024];
char feedback[1024];

const char *LTE = "wwan0";
const char *WiFi = "wlan0";
char s[INET_ADDRSTRLEN];