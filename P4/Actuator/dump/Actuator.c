#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

struct sockaddr_in ServerLTE;   // a socket struct design to be used with IPv4
struct sockaddr_in ServerWiFi;  // a socket struct design to be used with IPv4
struct sockaddr_storage remote_addr;
socklen_t fromlen;
int rv;
int sockLTE, sockWiFi;
int bindLTE, bindWiFi;
int lenLTE = sizeof(ServerLTE);
int lenWiFi = sizeof(ServerWiFi);
int rc_LTE, rc_WiFi;
int tx_LTE, tx_WiFI;
pthread_t T1, T2;
char buf[1024];
char buf2[1024];

const char *LTE = "wwan0";
const char *WiFi = "wlan0";

#define LTE_PORT 6969
#define WiFi_PORT 6968

#define LTE_ip "10.20.0.16"
#define WiFi_ip "192.168.1.136"

void transmit_LTE(char buf)
{
    tx_LTE = sendto(sockLTE, buf, strlen(buf), 0, (struct sockaddr *)&ServerLTE, lenLTE);
    printf("data from LTE \n \n");
}
void transmit_WiFI(char buf) 
{
    tx_WiFI = sendto(sockWiFi, buf, strlen(buf), 0, (struct sockaddr *)&ServerWiFi, lenWiFi);
    printf("Data from WiFi\n \n");
}


void receive_LTE()
{
    rc_LTE = recvfrom(sockLTE, buf2, strlen(buf2), (struct sockaddr *)&ServerLTE, &lenLTE);
    
}



int main() 
{
    int sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    //bind to device
    setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    ServerLTE.sin_family = AF_INET;
    ServerLTE.sin_port = htons(LTE_PORT);
    ServerLTE.sin_addr.s_addr = inet_addr(LTE_ip);

    ServerWiFi.sin_family = AF_INET;
    ServerWiFi.sin_port = htons(WiFi_PORT);
    ServerWiFi.sin_addr.s_addr = inet(WiFi_ip);

    //bindLTE = bind(sockLTE, (struct sockadd *)&ServerLTE, sizeof(struct sockaddr));
    //bindWiFi = bind (sockWiFi, (struct sockaddr *)&ServerWiFi, sizeof(struct sockaddr));

    transmit_LTE("Did you get this LTE-packet?");
    transmit_WiFI("did you get this WiFi-packet");


}