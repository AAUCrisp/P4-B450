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

#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "ExecutionVariable.h"
#include "shm_read_write.h"

using namespace std;

typedef struct _sockets {
    /* Receiver sockets*/
    int sockLTE_RECEIVER;
    int sockWiFi_RECEIVER;
    struct sockaddr_in ServerLTE_RECEIVER;
    struct sockaddr_in ServerWiFi_RECEIVER;

    /* Transmitter sockets*/
    int sockLTE_TRANSMITTER;
    int sockWiFi_TRANSMITTER;
    struct sockaddr_in ClientLTE_TRANSMITTER;
    struct sockaddr_in ClientWiFi_TRANSMITTER;
} Sockets;

/* Troubleshooting Options */
int print_COMMANDS = 1;

/* Define buffers & PORT number */
#define BUFFER 1024
#define SHM_BUFFER 100
char message[BUFFER];
char curr_time[128];
char *curr_timeLTE;
char *curr_timeWiFi;
char *receive;

/* Misc */
int bindLTE, bindWiFi;
int RX_LTE, RX_WiFi;
int TX_LTE, TX_WiFi;

char temp_msg[BUFFER];

/* Define threads */
pthread_t T1, T2;

std::ofstream File;

// File shit
FILE *fp1;
FILE *fp2;

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
    // setsockopt(sock->sockLTE_RECEIVER, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));
    // setsockopt(sock->sockWiFi_RECEIVER, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));

    /* Error checking */
    if (sock->sockLTE_RECEIVER == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sock->sockWiFi_RECEIVER == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    printf("Sockets_RECEIVER sucessfully created\n\n");

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
    printf("Sockets_Transmitter LTE (INSIDE): %d\n", sock->sockLTE_TRANSMITTER);
    printf("Sockets_Transmitter WiFi (INSIDE): %d\n\n", sock->sockWiFi_TRANSMITTER);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE_TRANSMITTER, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_TRANSMITTER, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Error checking */
    if (sock->sockLTE_TRANSMITTER == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sock->sockWiFi_TRANSMITTER == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    printf("Sockets_TRANSMITTER sucessfully created\n\n");

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
    Sockets *sock = (Sockets *)socket;
    const char *COMMANDS_KEY = "COMMANDS_KEY";
    char *writer = (char *)shm_write(SHM_BUFFER, COMMANDS_KEY);
    unsigned int LenLTE = sizeof(sock->ServerLTE_RECEIVER);

    // File.open("log.txt", std::ofstream::out | std::ofstream::app);

    while (1) {
        fp1 = fopen("Logs/log.txt", "a+");
        printf("receiveLTE socket: %d\n", sock->sockLTE_RECEIVER);

        cout << "EXECUTION variable: " << EXECUTION << "\n";

        printf("RX_LTE before: %d\n", RX_LTE);
        RX_LTE = recvfrom(sock->sockLTE_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE_RECEIVER, &LenLTE);
        printf("RX_LTE after: %d\n", RX_LTE);

        if (testvar != 0) {
            EXECUTION = true;
            cout << "EXECUTION variable: " << EXECUTION << "\n";
            Timestamp();

            if (print_COMMANDS == 1) {
                // printf("LTE || LTE-Thread id = %ld\n", pthread_self());
                printf("LTE || Message from LTE received at: %s\n", curr_time);
                printf("LTE || Message: %s from Control Unit \n\n", message);
            }
            sprintf(writer, "%s", message);
            /*File << "\n\n"
                 << "Received at: " << curr_time << "\n"
                 << message << " LTE";
            File.close();*/

            // fputs("Received at: ", fp1);
            // fputs(curr_time, fp1);
            // fputs("\n", fp1);
            // fputs(message, fp1);
            // fputs(" LTE\n\n", fp1);

            // fprintf(fp1, "Received at: %s\n %s    LTE : ", curr_time, message);

            // fprintf(fp1, "%s %s\n%s %s\n\n", "Received at:", curr_time, message, "LTE");
            fprintf(fp1, "%s %s %s\n", message, curr_time, "LTE");

            fclose(fp1);
            // File.close();
        } else {
            EXECUTION = false;
            cout << "EXECUTION variable: " << EXECUTION << "\n";
        }
    }
}

/* Function to receive WiFi packets */
void *receiveWiFi(void *socket) {
    Sockets *sock = (Sockets *)socket;
    const char *COMMANDS_KEY = "COMMANDS_KEY";
    char *writer = (char *)shm_write(SHM_BUFFER, COMMANDS_KEY);
    unsigned int LenWiFi = sizeof(sock->ServerWiFi_RECEIVER);

    // File.open("log.txt", std::ofstream::out | std::ofstream::app);

    while (1) {
        fp2 = fopen("Logs/log.txt", "a+");
        printf("receiveWiFi socket: %d\n", sock->sockWiFi_RECEIVER);

        cout << "start EXECUTION variable: " << EXECUTION << "\n";

        printf("RX_WiFi before: %d\n", RX_WiFi);
        RX_WiFi = recvfrom(sock->sockWiFi_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi_RECEIVER, &LenWiFi);

        if (message != temp_msg) {
            strcpy(temp_msg, message);
            EXECUTION = true;
            cout << "if EXECUTION variable: " << EXECUTION << "\n";
            testvar = 1;
            cout << "testvar: " << testvar << "\n";
        } else if (message == temp_msg && testvar == 0) {
            EXECUTION = false;
            cout << "else if EXECUTION variable: " << EXECUTION << "\n";
        }
        testvar = 0;
        cout << "testvar: " << testvar << "\n";

        Timestamp();

        if (print_COMMANDS == 1) {
            // printf("WiFi || WiFi-Thread id = %ld\n", pthread_self());
            printf("WiFi || Message from WiFi received at: %s \n", curr_time);
            printf("WiFi || Message: %s from Control Unit \n\n", message);
        }
        sprintf(writer, "%s", message);
        /*File << "\n\n"
             << "Received at: " << curr_time << "\n"
             << message << " WiFi";*/

        // fputs("Received at: ", fp2);
        // fputs(curr_time, fp2);
        // fputs("\n", fp2);
        // fputs(message, fp2);
        // fputs(" WiFi\n\n", fp2);

        // fprintf(fp2, "%s %s\n%s %s\n\n", "Received at:", curr_time, message, "WiFi");
        fprintf(fp2, "%s %s %s\n", message, curr_time, "WiFi");

        fclose(fp2);
        // File.close();
    }
}