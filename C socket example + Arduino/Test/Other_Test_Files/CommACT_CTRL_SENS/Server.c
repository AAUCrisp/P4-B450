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
char Message[BUFFER];
uint PORT_LTE1 = 9002;   // Receiver LTE
uint PORT_WiFi1 = 9003;  // Receiver WiFi

uint PORT_LTE2 = 9000;   // Transmitter LTE
uint PORT_WiFi2 = 9001;  // Transmitter WiFi

/* Specify LTE & WiFi interface */
const char *LTE = "wwan0";
const char *WiFi = "wlan0";
const char *IP_LTE = "10.20.0.16";
const char *IP_WiFi = "192.168.1.131";

/* Misc */
struct sockaddr_in ServerLTE1;
struct sockaddr_in ServerWiFi1;
struct sockaddr_in ServerLTE2;
struct sockaddr_in ServerWiFi2;
int sockLTE1, lenLTE1 = sizeof(ServerLTE1);
int sockWiFi1, lenWiFi1 = sizeof(ServerWiFi1);
int sockLTE2, lenLTE2 = sizeof(ServerLTE2);
int sockWiFi2, lenWiFi2 = sizeof(ServerWiFi2);
int bindLTE1, bindWiFi1;
int RX_LTE1, RX_WiFi1;
int TX_LTE1, TX_WiFi1;
pthread_t T1, T2, T3, T4;

/* Function to receive LTE packets */
void *receiveLTE() {
    RX_LTE1 = recvfrom(sockLTE1, Message, BUFFER, 0, (struct sockaddr *)&ServerLTE1, &lenLTE1);
    printf("LTE-Thread id = %ld\n", pthread_self());
    printf("From LTE1: %s\n\n", Message);
    pthread_exit(NULL);
}

/* Function to receive WiFi packets */
void *receiveWiFi() {
    RX_WiFi1 = recvfrom(sockWiFi1, Message, BUFFER, 0, (struct sockaddr *)&ServerWiFi1, &lenWiFi1);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("From WiFi1: %s\n\n", Message);
    pthread_exit(NULL);
}

void *transmitLTE(void *message) {
    char transmitBuffer[BUFFER];
    char *msg = message;
    sprintf(transmitBuffer, "%s", msg);
    // printf("TransmiteLTE: %s\n", transmitBuffer);
    sendto(sockLTE2, transmitBuffer, BUFFER, 0, (struct sockaddr *)&ServerLTE2, lenLTE2);
    // printf("LTE-Thread id = %ld\n\n", pthread_self());
    pthread_exit(NULL);
}

void *transmitWiFi(void *message) {
    char transmitBuffer[BUFFER];
    char *msg = message;
    sprintf(transmitBuffer, "%s", msg);
    // printf("TransmiteLTE: %s\n", transmitBuffer);
    sendto(sockWiFi2, transmitBuffer, BUFFER, 0, (struct sockaddr *)&ServerWiFi2, lenWiFi2);
    // printf("WiFi-Thread id = %ld\n\n", pthread_self());
    pthread_exit(NULL);
}

int main() {
    /* Receive Socket */
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

    /* --------------------------------------------------------- */
    /* Transmit Socket */
    sockLTE2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockWiFi2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    setsockopt(sockLTE2, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockWiFi2, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    if (sockLTE2 == -1) {
        perror("Failed to create sockLTE1");
        exit(0);
    }
    if (sockWiFi2 == -1) {
        perror("Failed to create sockWiFi1");
        exit(0);
    }

    ServerLTE2.sin_family = AF_INET;
    ServerLTE2.sin_port = htons(PORT_LTE2);
    ServerLTE2.sin_addr.s_addr = inet_addr(IP_LTE);

    ServerWiFi2.sin_family = AF_INET;
    ServerWiFi2.sin_port = htons(PORT_WiFi2);
    ServerWiFi2.sin_addr.s_addr = inet_addr(IP_WiFi);

    // char msg[] = "Hello back from WiFi";
    // char msg2[] = "Hello back from LTE";

    char msg[] = "Hello back from LTE! Control Unit";
    char msg2[] = "Hello back from WiFi! Control Unit";
    printf("Just to make sure it prints");
    while (1) {
        // pthread_create(&T1, NULL, receiveLTE, NULL);
        // pthread_create(&T2, NULL, receiveWiFi, NULL);
        // pthread_create(&T3, NULL, transmitLTE, msg);
        // pthread_create(&T4, NULL, transmitWiFi, msg2);
        printf("Just to make sure it prints");
        sendto(sockWiFi2, msg, BUFFER, 0, (struct sockaddr *)&ServerWiFi2, lenWiFi2);
        sendto(sockLTE2, msg2, BUFFER, 0, (struct sockaddr *)&ServerLTE2, lenLTE2);
        printf("Just to make sure it prints");
        sleep(3);

        // sendto(sockLTE2, msg, BUFFER, 0, (struct sockaddr *)&ServerLTE2, lenLTE2);
        // printf("Does it reach here? LTE");
        // sendto(sockWiFi2, msg2, BUFFER, 0, (struct sockaddr *)&ServerWiFi2, lenWiFi2);
        // printf("Does it reach here? WiFi");

        // RX_LTE1 = recvfrom(sockLTE1, Message, BUFFER, 0, (struct sockaddr *)&ServerLTE1, &lenLTE1);
        // printf("From LTE1: %s\n", Message);
        // RX_WiFi1 = recvfrom(sockWiFi1, Message, BUFFER, 0, (struct sockaddr *)&ServerWiFi1, &lenWiFi1);
        // printf("From WiFi1: %s\n", Message);

        //  pthread_join(T1, NULL);
        //  pthread_join(T2, NULL);
    }
}
