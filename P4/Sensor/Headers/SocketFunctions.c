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

#include "SocketFunctions.h"
#include "shm_write_read.h"


/* Troubleshooting Options */
int print_GSV = 1;
int print_out = 1;

/* Define buffers & PORT number */
#define BUFFER 1024
char message[BUFFER];
char curr_time[128];
char *curr_timeLTE;
char *curr_timeWiFi;
char *receive;

/* Misc */
int bindLTE, bindWiFi;
int RX_LTE, RX_WiFi;
int TX_LTE, TX_WiFi;

/* Define threads */
pthread_t T1, T2;


/* Function to create receiver sockets */
void Sockets_Receiver(Sockets *sock, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi) {
    /* Time struct for socket timeout */
    struct timeval tv2;
    tv2.tv_sec = 0;
    tv2.tv_usec = 500000;
    
    /* Create socket receiver */
    sock->sockLTE_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("Sockets_Receiver LTE (INSIDE): %d\n", sock->sockLTE_RECEIVER);
    printf("Sockets_Receiver WiFi (INSIDE): %d\n\n", sock->sockWiFi_RECEIVER);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));
    //setsockopt(sock->sockLTE_RECEIVER, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));
    //setsockopt(sock->sockWiFi_RECEIVER, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));

    /* Error checking */
    if (sock->sockLTE_RECEIVER == -1) {
        perror("Failed to create LTE Socket");
        exit(0);
    }
    if (sock->sockWiFi_RECEIVER == -1) {
        perror("Failed to create WiFi Socket");
        exit(0);
    }
    printf("Receiver Sockets sucessfully created\n\n");

    /* Configure settings to communicate with remote UDP client for receiver */
    sock->ServerLTE_RECEIVER.sin_family = AF_INET;
    sock->ServerLTE_RECEIVER.sin_port = htons(PORT_LTE);
    sock->ServerLTE_RECEIVER.sin_addr.s_addr = INADDR_ANY;

    sock->ServerWiFi_RECEIVER.sin_family = AF_INET;
    sock->ServerWiFi_RECEIVER.sin_port = htons(PORT_WiFi);
    sock->ServerWiFi_RECEIVER.sin_addr.s_addr = INADDR_ANY;

    /* Bind to socket */
    bindLTE = bind(sock->sockLTE_RECEIVER, (struct sockaddr *)&sock->ServerLTE_RECEIVER, sizeof(struct sockaddr));
    bindWiFi = bind(sock->sockWiFi_RECEIVER, (struct sockaddr *)&sock->ServerWiFi_RECEIVER, sizeof(struct sockaddr));

    /* Error checking */
    if (bindLTE == -1) {
        perror("Failed to bind LTE socket");
        exit(0);
    }
    if (bindWiFi == -1) {
        perror("Failed to bind WiFi socket");
        exit(0);
    }
    printf("Bind was succesful!\n\n");
}

/* Function to create transmitter sockets */
void Sockets_Transmitter(Sockets *sock, const char *IP_LTE, const char *IP_WiFi, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi) {
    /* Create socket receiver */
    sock->sockLTE_TRANSMITTER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi_TRANSMITTER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("Transmit Socket LTE (INSIDE): %d\n", sock->sockLTE_TRANSMITTER);
    printf("Transmit Socket WiFi (INSIDE): %d\n\n", sock->sockWiFi_TRANSMITTER);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE_TRANSMITTER, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_TRANSMITTER, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Error checking */
    if (sock->sockLTE_TRANSMITTER == -1) {
        perror("Failed to create LTE Socket");
        exit(0);
    }
    if (sock->sockWiFi_TRANSMITTER == -1) {
        perror("Failed to create LTE Socket");
        exit(0);
    }
    printf("Transmit Sockets sucessfully created\n\n");

    /* Configure settings to communicate with remote UDP client for receiver */
    sock->ClientLTE_TRANSMITTER.sin_family = AF_INET;
    sock->ClientLTE_TRANSMITTER.sin_port = htons(PORT_LTE);
    sock->ClientLTE_TRANSMITTER.sin_addr.s_addr = inet_addr(IP_LTE);

    sock->ClientWiFi_TRANSMITTER.sin_family = AF_INET;
    sock->ClientWiFi_TRANSMITTER.sin_port = htons(PORT_WiFi);
    sock->ClientWiFi_TRANSMITTER.sin_addr.s_addr = inet_addr(IP_WiFi);
}

/* Function to timestamp packets */
char *Timestamp() {
    /* Timestamp format : [hh:mm:ss dd/mm/yy] */
    struct tm *timeinfo;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    timeinfo = localtime(&tv.tv_sec);

    sprintf(curr_time, "[%d:%d:%d.%03ld %d/%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tv.tv_usec / 1000, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    return curr_time;
}

int generate(int Min, int Max) {
    int number = (rand() % ((Max + 1) - Min)) + Min;
    return number;
}

/* Function to receive LTE packets */
void *receiveLTE(void *socket) {
    while (1) {
        Sockets *sock = (Sockets *)socket;
        const char *GSV_KEY = "GSV_KEY";
        int LenLTE = sizeof(sock->ServerLTE_RECEIVER);
        if (print_GSV == 1) {
            printf("GSV || LTE socket: %d\n", sock->sockLTE_RECEIVER);
        }
        RX_LTE = recvfrom(sock->sockLTE_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE_RECEIVER, &LenLTE);
        Timestamp();

        if (print_GSV == 1) {
            // printf("LTE || LTE-Thread id = %ld\n", pthread_self());
            printf("GSV || LTE || Message from LTE received at: %s\n", curr_time);
            printf("GSV || LTE || Message: %s from Control Unit \n\n", message);
        }
        shm_write(message, 32, GSV_KEY);
    }
}

/* Function to receive WiFi packets */
void *receiveWiFi(void *socket) {
    while (1) {
        Sockets *sock = (Sockets *)socket;
        const char *GSV_KEY = "GSV_KEY";
        int LenWiFi = sizeof(sock->ServerWiFi_RECEIVER);
        
        if (print_GSV == 1) {
            printf("GSV || WiFi socket: %d\n", sock->sockWiFi_RECEIVER);
        }
        RX_WiFi = recvfrom(sock->sockWiFi_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi_RECEIVER, &LenWiFi);
        Timestamp();

        if (print_GSV == 1) {
            // printf("WiFi || WiFi-Thread id = %ld\n", pthread_self());
            printf("GSV || WiFi || Message from WiFi received at: %s \n", curr_time);
            printf("GSV || WiFi || Message: %s from Control Unit \n\n", message);
        }
        shm_write(message, 32, GSV_KEY);
    }
}

void *transmitLTE(void *socket, char* message) {
    Sockets *sock = (Sockets *)socket;
    char sendLTE[BUFFER];
    

    if (print_out == 1) {
        printf("Sensor || LTE socket: %d\n", sock->sockLTE_TRANSMITTER);
    }

    if (print_out == 1) {
        printf("Sensor || LTE || Random int from shm: %s\n", message);
    }

    curr_timeLTE = Timestamp();
    sprintf(sendLTE, "%s %s", message, curr_timeLTE);
    if (print_out == 1) {
        printf("Sensor || LTE || sendLTE: %s\n", sendLTE);
    }


    sendto(sock->sockLTE_TRANSMITTER, sendLTE, BUFFER, 0, (struct sockaddr *)&sock->ClientLTE_TRANSMITTER, sizeof(sock->ClientLTE_TRANSMITTER));
    if (print_out == 1) {
        printf("Sensor || LTE || Message transmitted at %s\n\n", curr_timeLTE);
    }
    pthread_exit(NULL);
}

void *transmitWiFi(void *socket, char* message) {
    Sockets *sock = (Sockets *)socket;
    char sendWiFi[BUFFER];

    if (print_out == 1) {
        printf("Sensor || WiFi socket: %d\n", sock->sockLTE_TRANSMITTER);
    }

    if (print_out == 1) {
        printf("Sensor || WiFi || Random int from shm: %s\n", message);
    }

    curr_timeWiFi = Timestamp();
    sprintf(sendWiFi, "%s %s", message, curr_timeWiFi);
    if (print_out == 1) {
        printf("Sensor || WiFi || sendWiFi: %s\n", sendWiFi);
    }

    sendto(sock->sockWiFi_TRANSMITTER, sendWiFi, BUFFER, 0, (struct sockaddr *)&sock->ClientWiFi_TRANSMITTER, sizeof(sock->ClientWiFi_TRANSMITTER));
    if (print_out == 1) {
        printf("Sensor || WiFi || Message transmitted at %s\n\n", curr_timeWiFi);
    }
}
