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

struct sockaddr_in ClientLTE;   // a socket struct design to be used with IPv4
struct sockaddr_in ClientWiFi;  // a socket struct design to be used with IPv4
struct sockaddr_in ServerLTE, ServerWiFi;
struct sockaddr_storage remote_addr;
socklen_t fromlen;
int rv;
int sockLTE, sockWiFi, sockLTE2, sockWiFi2;
int bindLTE, bindWiFi;
int lenLTE = sizeof(ClientLTE);
int lenWiFi = sizeof(ClientWiFi);
int rc_LTE, rc_WiFi;
int tx_LTE, tx_WiFI;
pthread_t T1, T2, T3, T4;

const char *LTE = "wwan0";
const char *WiFi = "wlan0";

#define LTE_PORT 6969
#define WiFi_PORT 6968
#define LTE_PORT2 6967
#define WiFi_PORT2 6966

#define LTE_ip "10.20.0.16"
#define WiFi_ip "192.168.1.136"


//char *buf[] = "THIS IS LTE!";
//char *buf2[] = "THIS IS WIFI!";
char buf3[64];
char buf4[64];

void *transmit_LTE(void *msg) {
    tx_LTE = sendto(sockLTE, msg, 1024, 0, (struct sockaddr *)&ClientLTE, lenLTE);
    printf("LTE-Thread id = %ld\n", pthread_self());
    printf("Sending Data from LTE \n \n");
}
void *transmit_WiFi(void *msg2) {
    tx_WiFI = sendto(sockWiFi, msg2, 1024, 0, (struct sockaddr *)&ClientWiFi, lenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("Sending Data from WiFi\n \n");
}

void *receiveLTE1() {
    rc_LTE = recvfrom(sockLTE2, buf3, sizeof(buf3), 0, (struct sockaddr *)&ClientLTE, &lenLTE);
    printf("LTE-Thread id = %ld\n", pthread_self());
    printf("Receiving Data From LTE: %s\n", buf3);
    pthread_exit(NULL);
}

/* Function to receive WiFi packets */
void *receiveWiFi1() {
    rc_WiFi = recvfrom(sockWiFi2, buf4, sizeof(buf4), 0, (struct sockaddr *)&ClientWiFi, &lenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("Receiving Data from WiFi: %s\n", buf4);
    pthread_exit(NULL);
}

int main() {
    int sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int sockLTE2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int sockWiFi2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // bind to device
    setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockLTE2, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));
    setsockopt(sockWiFi2, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));


    ClientLTE.sin_family = AF_INET;
    ClientLTE.sin_port = htons(LTE_PORT);
    ClientLTE.sin_addr.s_addr = inet_addr(LTE_ip);

    ServerLTE.sin_family = AF_INET;
    ServerLTE.sin_port = htons(LTE_PORT2);
    ServerLTE.sin_addr.s_addr = INADDR_ANY;

    ClientWiFi.sin_family = AF_INET;
    ClientWiFi.sin_port = htons(WiFi_PORT);
    ClientWiFi.sin_addr.s_addr = inet_addr(WiFi_ip);

    ServerWiFi.sin_family = AF_INET;
    ServerWiFi.sin_port = htons(WiFi_PORT2);
    ServerWiFi.sin_addr.s_addr = INADDR_ANY;
    

    bindLTE = bind(sockLTE2, (struct sockaddr *)&ServerLTE, sizeof(struct sockaddr));
    bindWiFi = bind (sockWiFi2, (struct sockaddr *)&ServerWiFi, sizeof(struct sockaddr));
    char *buf[] = "THIS IS LTE!";
    char *buf2[] = "THIS IS WIFI!";

    while (1) {
        
        usleep(500000);
        /*
        tx_LTE = sendto(sockLTE, buf, sizeof(buf), 0, (struct sockaddr *)&ClientLTE, lenLTE);
        printf("data from LTE \n \n");
        tx_WiFI = sendto(sockWiFi, buf2, sizeof(buf2), 0, (struct sockaddr *)&ClientWiFi, lenWiFi);
        printf("Data from WiFi\n \n");
        */
        // rc_LTE = recvfrom(sockLTE, buf3, sizeof(buf3), 0, (struct sockaddr *)&ClientLTE, &lenLTE);
        // printf("From LTE: %s\n", buf3);
        // rc_WiFi = recvfrom(sockWiFi, buf4, sizeof(buf4), 0, (struct sockaddr *)&ClientWiFi, &lenWiFi);
        // printf("From WiFi: %s\n", buf4);
        
        pthread_create(&T1, NULL, receiveLTE1, NULL);
        pthread_create(&T2, NULL, receiveWiFi1, NULL);
        pthread_create(&T3, NULL, transmit_LTE, buf);
        pthread_create(&T4, NULL, transmit_WiFi, buf2);
    }
}
