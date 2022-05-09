#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>



/* Function to check RSSI value */
void Check_RSSI_Value() {
    /*
    1. Check RSSI value from LTE/WiFi.
    2. If LTE/WiFi is better than the other.
    3. Update Global Signal Variable (GSV).
    4. Notify client & send GSV to client via the better signal.
    */

    if (RSSI == 1) {
        update GSV = 1;
        sendto(sockLTE, GSV, BUFFER, 0, (struct sockaddr *)&ClientLTE, lenLTE);
    }
    if (RSSI == 2) {
        update GSV = 2;
        sendto(sockWiFi, GSV, BUFFER, 0, (struct sockaddr *)&ClientWiFi, lenWiFi);
    }
}

void Process_Data() {
    /*
    1. Receive data from client.
    2. Process data from client.
    3. Create a command based on data.
    4. Send command to client.
    */
}

/* Main running code */
int main() {
    /* Initialize PORT & INTERFACE*/
    PORT_LTE = 9123;
    PORT_WiFi = 9124;
    LTE = "wwan0";
    WiFi = "wlan0";

    /* Create sockets */
    Create_Bind_Sockets(PORT_LTE, PORT_WiFi);

    while (1) {
        Timestamp();
        pthread_create(&T1, NULL, receiveshit1, NULL);
        pthread_create(&T2, NULL, receiveshit2, NULL);
    }

    close(sockLTE && sockWiFi);
    exit(0);
}






/* Define buffers & PORT number 
#define BUFFER 128
char Message[BUFFER];
uint PORT_LTE, PORT_WiFi;
char curr_time[BUFFER];

/* Specify LTE & WiFi interface 
const char *LTE;
const char *WiFi;

/* Misc 
struct sockaddr_in ServerLTE;
struct sockaddr_in ServerWiFi;
int sockLTE, lenLTE = sizeof(ServerLTE);
int sockWiFi, lenWiFi = sizeof(ServerWiFi);
int bindLTE, bindWiFi;
int RX_LTE, RX_WiFi;
int TX_LTE, TX_WiFi;
pthread_t T1, T2;

/* Global Signal Variable & RSSI 
int GSV;
int RSSI = 1;

/* Function to bind sockets 
void Create_Bind_Sockets(uint PORT_LTE, uint PORT_WiFi) {
    /* Create socket 
    sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* Setting up socket options & specifying interface
    setsockopt(sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Error checking 
    if (sockLTE == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sockWiFi == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }

    /* Configure settings to communicate with remote UDP client 
    ServerLTE.sin_family = AF_INET;
    ServerLTE.sin_port = htons(PORT_LTE);
    ServerLTE.sin_addr.s_addr = INADDR_ANY;

    ServerWiFi.sin_family = AF_INET;
    ServerWiFi.sin_port = htons(PORT_WiFi);
    ServerWiFi.sin_addr.s_addr = INADDR_ANY;

    /* Bind to socket 
    bindLTE = bind(sockLTE, (struct sockaddr *)&ServerLTE, sizeof(struct sockaddr));
    bindWiFi = bind(sockWiFi, (struct sockaddr *)&ServerWiFi, sizeof(struct sockaddr));

    /* Error checking 
    if (bindLTE == -1) {
        perror("Failed to bind LTE socket");
        exit(0);
    }
    if (bindWiFi == -1) {
        perror("Failed to bind WiFi socket");
        exit(0);
    }
    printf("Bind was succesful!\n");
}

/* Function to receive LTE packets 
void *receiveshit1() {
    RX_LTE = recvfrom(sockLTE, Message, BUFFER, 0, (struct sockaddr *)&ServerLTE, &lenLTE);
    printf("LTE-Thread id = %ld\n", pthread_self());
    printf("%s\n", Message);
    printf("Message from LTE received at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

/* Function to receive WiFi packets 
void *receiveshit2() {
    RX_WiFi = recvfrom(sockWiFi, Message, BUFFER, 0, (struct sockaddr *)&ServerWiFi, &lenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("%s\n", Message);
    printf("Message from WiFi received at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

/* Function to transmit LTE packets 
void *transmitshit1() {
    TX_LTE = sendto(sockLTE, GSV, BUFFER, 0, (struct sockaddr *)&ClientLTE, lenLTE);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("%s\n", Message);
    printf("Message from WiFi transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

/* Function to transmit WiFi packets 
void *transmitshit2() {
    TX_WiFi = sendto(sockWiFi, GSV, BUFFER, 0, (struct sockaddr *)&ClientWiFi, lenWiFi);
    printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("%s\n", Message);
    printf("Message from WiFi transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

/* Function to timestamp packets 
Timestamp format : [hh:mm:ss dd/mm/yy]
 
char *Timestamp() {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(curr_time, "[%d:%d:%d %d/%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    return curr_time;
}*/