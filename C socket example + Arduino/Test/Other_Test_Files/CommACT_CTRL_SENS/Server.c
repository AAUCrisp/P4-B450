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
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Define buffers & PORT number */
#define BUFFER 128
#define LTE_IP "10.20.0.16"
#define WiFi_IP "192.168.1.136"
char Message[BUFFER];
uint PORT_LTE1 = 6969;
uint PORT_WiFi1 = 6968;
uint PORT_LTE2 = 6967;
uint PORT_WiFi2 = 6966;


/* Specify LTE & WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";

/* Misc */
struct sockaddr_in ServerLTE1;
struct sockaddr_in ServerWiFi1;
int sockLTE1, lenLTE1 = sizeof(ServerLTE1);
int sockWiFi1, lenWiFi1 = sizeof(ServerWiFi1);
int bindLTE1, bindWiFi1;
int RX_LTE1, RX_WiFi1;
int TX_LTE1, TX_WiFi1;
pthread_t T1, T2, T3, T4;

/* Function to receive LTE packets */
void *receiveLTE1() {
    RX_LTE1 = recvfrom(sockLTE1, Message, BUFFER, 0, (struct sockaddr *)&ServerLTE1, &lenLTE1);
    printf("LTE-Thread id = %ld\n", pthread_self());
    printf("From LTE1: %s\n", Message);
    //char msg[] = "Hello back from LTE";
    //sendto(sockLTE1, msg, BUFFER, 0, (struct sockaddr *)&ServerLTE1, lenLTE1);

    pthread_exit(NULL);
}

/* Function to receive WiFi packets */
void *receiveWiFi1() {
    RX_WiFi1 = recvfrom(sockWiFi1, Message, BUFFER, 0, (struct sockaddr *)&ServerWiFi1, &lenWiFi1);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("From WiFi1: %s\n", Message);
    //char msg[] = "Hello back from WiFi";
    //sendto(sockWiFi1, msg, BUFFER, 0, (struct sockaddr *)&ServerWiFi1, lenWiFi1);
    pthread_exit(NULL);
}

int main() {
    sockLTE1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockWiFi1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    setsockopt(sockLTE1, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockWiFi1, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    if (sockLTE1 == -1) {
        perror("Failed to create sockLTE1");
        exit(0);
    }
    if (sockWiFi1 == -1) {
        perror("Failed to create sockWiFi1");
        exit(0);
    }

    ServerLTE1.sin_family = AF_INET;
    ServerLTE1.sin_port = htons(PORT_LTE1);
    ServerLTE1.sin_addr.s_addr = INADDR_ANY;

    ServerWiFi1.sin_family = AF_INET;
    ServerWiFi1.sin_port = htons(PORT_WiFi1);
    ServerWiFi1.sin_addr.s_addr = INADDR_ANY;

    bindLTE1 = bind(sockLTE1, (struct sockaddr *)&ServerLTE1, sizeof(struct sockaddr));
    printf("Bind to sockLTE1 was successful\n");
    bindWiFi1 = bind(sockWiFi1, (struct sockaddr *)&ServerWiFi1, sizeof(struct sockaddr));
    printf("Bind to sockWiFi1 was successful\n");
    char msg[] = "Hello back from WiFi";
    char msg2[] = "Hello back from LTE";
    while (1) {
        //pthread_create(&T1, NULL, receiveLTE1, NULL);
        //pthread_create(&T2, NULL, receiveWiFi1, NULL);
        RX_LTE1 = recvfrom(sockLTE1, Message, BUFFER, 0, (struct sockaddr *)&ServerLTE1, &lenLTE1);
        printf("From LTE1: %s\n", Message);
        RX_WiFi1 = recvfrom(sockWiFi1, Message, BUFFER, 0, (struct sockaddr *)&ServerWiFi1, &lenWiFi1);
        printf("From WiFi1: %s\n", Message);
        
        sendto(sockWiFi1, msg, BUFFER, 0, (struct sockaddr *)&ServerWiFi1, lenWiFi1);
        sendto(sockLTE1, msg2, BUFFER, 0, (struct sockaddr *)&ServerLTE1, lenLTE1);
        // pthread_join(T1, NULL);
        // pthread_join(T2, NULL);
    }
}