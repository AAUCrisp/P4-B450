#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

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
int tx_LTE, tx_WiFi;
pthread_t T1, T2;

const char *LTE = "wwan0";
const char *WiFi = "wlan0";

#define LTE_PORT 6969
#define WiFi_PORT 6968

#define LTE_ip "10.20.0.16"
#define WiFi_ip "192.168.1.136"

char buf[] = "THIS IS LTE!";
char buf2[] = "THIS IS WIFI!";
char buf3[64];
char buf4[64];

void transmit_LTE(char *buf) {
    tx_LTE = sendto(sockLTE, buf, 1024, 0, (struct sockaddr *)&ServerLTE, lenLTE);
    printf("data from LTE \n \n");
}
void transmit_WiFI(char *buf) {
    tx_WiFi = sendto(sockWiFi, buf, 1024, 0, (struct sockaddr *)&ServerWiFi, lenWiFi);
    printf("Data from WiFi\n \n");
}

void *receiveLTE1() {
    tx_LTE = sendto(sockLTE, buf, sizeof(buf), 0, (struct sockaddr *)&ServerLTE, lenLTE);
    printf("data from LTE \n \n");
    rc_LTE = recvfrom(sockLTE, buf3, sizeof(buf3), 0, (struct sockaddr *)&ServerLTE, &lenLTE);
    printf("LTE-Thread id = %ld\n", pthread_self());
    printf("From LTE: %s\n", buf3);
    pthread_exit(NULL);
}

/* Function to receive WiFi packets */
void *receiveWiFi1() {
    tx_WiFi = sendto(sockWiFi, buf2, sizeof(buf2), 0, (struct sockaddr *)&ServerWiFi, lenWiFi);
    printf("Data from WiFi\n \n");
    rc_WiFi = recvfrom(sockWiFi, buf4, sizeof(buf4), 0, (struct sockaddr *)&ServerWiFi, &lenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("From WiFi: %s\n", buf4);
    pthread_exit(NULL);
}

int main() {
    int sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // bind to device
    setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    ServerLTE.sin_family = AF_INET;
    ServerLTE.sin_port = htons(LTE_PORT);
    ServerLTE.sin_addr.s_addr = inet_addr(LTE_ip);

    ServerWiFi.sin_family = AF_INET;
    ServerWiFi.sin_port = htons(WiFi_PORT);
    ServerWiFi.sin_addr.s_addr = inet_addr(WiFi_ip);

    // bindLTE = bind(sockLTE, (struct sockadd *)&ServerLTE, sizeof(struct sockaddr));
    // bindWiFi = bind (sockWiFi, (struct sockaddr *)&ServerWiFi, sizeof(struct sockaddr));
    

    while (1) {
        usleep(500000);
        // rc_LTE = recvfrom(sockLTE, buf3, sizeof(buf3), 0, (struct sockaddr *)&ServerLTE, &lenLTE);
        // printf("From LTE: %s\n", buf3);
        // rc_WiFi = recvfrom(sockWiFi, buf4, sizeof(buf4), 0, (struct sockaddr *)&ServerWiFi, &lenWiFi);
        // printf("From WiFi: %s\n", buf4);
        pthread_create(&T1, NULL, receiveLTE1, NULL);
        pthread_create(&T2, NULL, receiveWiFi1, NULL);
    }
}
