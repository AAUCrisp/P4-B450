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

#include "ActuatorFunctions.h"
//#include "ExecutionVariable.h"
#include "shm_read_write.h"

using namespace std;

typedef struct _sockets {
    /* Receiver sockets */
    int sockLTE_RECEIVER;
    int sockWiFi_RECEIVER;
    struct sockaddr_in ServerLTE_RECEIVER;
    struct sockaddr_in ServerWiFi_RECEIVER;

    /* Transmitter sockets */
    int sockLTE_TRANSMITTER;
    int sockWiFi_TRANSMITTER;
    struct sockaddr_in ClientLTE_TRANSMITTER;
    struct sockaddr_in ClientWiFi_TRANSMITTER;

    /* Execution timing variable */
    int packet_count_LTE;
    int packet_count_WiFi;
    int fail_count_LTE;
    int fail_count_WiFi;
    long double Execution_Sum_LTE;
    long double Execution_Sum_WiFi;
    int STOP_LTE;
    int STOP_WiFi;

} Sockets;

/* Troubleshooting Options */
int print_COMMANDS = 1;

/* Define buffers & PORT number */
#define BUFFER 1024
#define SHM_BUFFER 1024
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
    tv2.tv_sec = 5;
    tv2.tv_usec = 0;

    /* Create socket receiver */
    sock->sockLTE_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("Sockets_Receiver LTE (INSIDE): %d\n", sock->sockLTE_RECEIVER);
    printf("Sockets_Receiver WiFi (INSIDE): %d\n\n", sock->sockWiFi_RECEIVER);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));
    setsockopt(sock->sockLTE_RECEIVER, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));
    setsockopt(sock->sockWiFi_RECEIVER, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));

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
    unsigned int LenLTE = sizeof(sock->ServerLTE_RECEIVER);

    /* Execution time variables */
    struct timespec begin, end;
    unsigned long seconds = 0;
    unsigned long nanoseconds = 0;
    double elapsed = 0;

    sock->packet_count_LTE = 0;
    sock->fail_count_LTE = 0;
    sock->Execution_Sum_LTE = 0;
    sock->STOP_LTE = 0;

    while (sock->STOP_LTE != 1) {
        // printf("receiveLTE socket: %d\n", sock->sockLTE_RECEIVER);
        RX_LTE = recvfrom(sock->sockLTE_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE_RECEIVER, &LenLTE);
        printf("RX_LTE: %d\n", RX_LTE);
        if (RX_LTE == -1) {
            sock->STOP_LTE = 1;
            sleep(1);
            cout << "\n\n====================================================="
                 << endl;
            cout << "\nLTE : Execution_Sum: " << sock->Execution_Sum_LTE << endl;
            printf("LTE : Total failed counts: %d\n", sock->fail_count_LTE);
            printf("LTE : Total packets received via LTE: %d\n\n", sock->packet_count_LTE);
            cout << "=====================================================\n\n"
                 << endl;
            sleep(10);
            return 0;
        } else {
            sock->STOP_LTE = 0;
            printf("Do I reach this if RX_LTE != -1?\n");
        }
        Timestamp();

        fp1 = fopen("Logs/log.txt", "a+");
        fprintf(fp1, "%s %s %s\n", message, curr_time, "LTE");
        fclose(fp1);

        if (print_COMMANDS == 1) {
            // printf("LTE || LTE-Thread id = %ld\n", pthread_self());
            printf("I reach this after RX_LTE == -1\n");
            printf("LTE || Message from LTE received at: %s\n", curr_time);
            printf("LTE || Message: %s from Control Unit \n\n", message);
        }

        /* Start timing code execution of code */
        clock_gettime(CLOCK_REALTIME, &begin);

        processData(message);

        /* Stop timing code execution of code */
        clock_gettime(CLOCK_REALTIME, &end);

        seconds = end.tv_sec - begin.tv_sec;
        nanoseconds = end.tv_nsec - begin.tv_nsec;

        /* Calculation of elapsed time sum */
        elapsed = seconds + nanoseconds * 1e-9;
        if (elapsed > 10000) {
            sock->fail_count_LTE++;
            elapsed = 0;
        }
        sock->Execution_Sum_LTE += elapsed;
        sock->packet_count_LTE++;
    }
}

/* Function to receive WiFi packets */
void *receiveWiFi(void *socket) {
    Sockets *sock = (Sockets *)socket;
    unsigned int LenWiFi = sizeof(sock->ServerWiFi_RECEIVER);

    /* Execution time variables */
    struct timespec begin, end;
    unsigned long seconds = 0;
    unsigned long nanoseconds = 0;
    double elapsed = 0;

    sock->packet_count_WiFi = 0;
    sock->fail_count_WiFi = 0;
    sock->Execution_Sum_WiFi = 0;
    sock->STOP_WiFi = 0;

    while (sock->STOP_WiFi != 1) {
        // printf("receiveWiFi socket: %d\n", sock->sockWiFi_RECEIVER);

        RX_WiFi = recvfrom(sock->sockWiFi_RECEIVER, message, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi_RECEIVER, &LenWiFi);
        printf("RX_WiFi: %d\n", RX_LTE);
        if (RX_WiFi == -1) {
            sock->STOP_WiFi = 1;
            sleep(1);
            cout << "\n\n====================================================="
                 << endl;
            cout << "\nWiFi : Execution_Sum: " << sock->Execution_Sum_WiFi << endl;
            printf("WiFi : Total failed counts: %d\n", sock->fail_count_WiFi);
            printf("WiFi : Total packets received via WiFi: %d\n\n", sock->packet_count_WiFi);
            cout << "=====================================================\n\n"
                 << endl;
            sleep(10);
            return 0;
        } else {
            sock->STOP_WiFi = 0;
            printf("Do I reach this if RX_WiFi != -1?\n");
        }
        Timestamp();

        fp2 = fopen("Logs/log.txt", "a+");
        fprintf(fp2, "%s %s %s\n", message, curr_time, "WiFi");
        fclose(fp2);

        if (print_COMMANDS == 1) {
            // printf("WiFi || WiFi-Thread id = %ld\n", pthread_self());
            printf("I reach this after RX_WiFi == -1\n");
            printf("WiFi || Message from WiFi received at: %s \n", curr_time);
            printf("WiFi || Message: %s from Control Unit \n\n", message);
        }

        /* Start timing code execution of code */
        clock_gettime(CLOCK_REALTIME, &begin);

        processData(message);

        /* Stop timing code execution of code */
        clock_gettime(CLOCK_REALTIME, &end);

        seconds = end.tv_sec - begin.tv_sec;
        nanoseconds = end.tv_nsec - begin.tv_nsec;

        /* Calculation of elapsed time sum */
        elapsed = seconds + nanoseconds * 1e-9;
        if (elapsed > 10000) {
            sock->fail_count_WiFi++;
            elapsed = 0;
        }
        sock->Execution_Sum_WiFi += elapsed;
        sock->packet_count_WiFi++;
    }
}