#ifndef LIBRARIES
#define LIBRARIES
#include "../Libraries.cpp"      // File with all our includes
#endif

#ifndef SHM_READ
#define SHM_READ
#include "shm_read.cpp"
#endif

// #ifndef SHM_WRITE
// #define SHM_WRITE

// #include "shm_write.cpp"

// #endif
#ifndef SOCKETS_NET
#define SOCKETS_NET
#include "Sockets.h"
#endif

// #ifndef CONVERTER
// #define CONVERTER
// #include "../converter.cpp"
// #endif

/* -------------------------
-------- SETUP AREA --------
---------- start ---------*/

#ifndef SOCKET_STRUCT
#define SOCKET_STRUCT

typedef struct _sockets {
        int sockLTE;
        int sockWiFi;
        struct sockaddr_in ServerLTE;
        struct sockaddr_in ServerWiFi;
        /* Transmit sockets */
        int sockLTE_transmit;
        int sockWiFi_transmit;
        struct sockaddr_in ServerLTE_transmit;
        struct sockaddr_in ServerWiFi_transmit;

    }Sockets;

typedef struct _time_struct{
    int hour;
    int minute;
    int second;
    int millis;
} time_struct;
#endif


/* Define buffers & PORT number */
#define BUFFER 1024
char message_LTE[BUFFER];
char message_WiFi[BUFFER];
char *receive_LTE;
char *receive_WiFi;
char curr_time[128];

/* Misc */
int bindLTE, bindWiFi;
int RX_LTE, RX_WiFi;
int TX_LTE, TX_WiFi;

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
/* -------- end ------------
-------- SETUP AREA --------
--------------------------*/

/* -------------------------
------- RECEIVE AREA -------
---------- start ---------*/

void Sockets_Receiver(Sockets *sock, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi) {
    /* Time struct for socket timeout */
    struct timeval tv2;
    tv2.tv_sec = 0;
    tv2.tv_usec = 500000;
    
    /* Create socket receiver */
    sock->sockLTE_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("Sockets_Receiver LTE (INSIDE): %d\n", sock->sockLTE_RECEIVER);
    printf("Sockets_Receiver WiFi (INSIDE): %d\n", sock->sockWiFi_RECEIVER);

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
    printf("Sockets_RECEIVER sucessfully created\n");

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
    printf("Bind was succesful!\n");
}


/* Function to receive LTE packets */
void *receiveLTE(void *socket) {
    Sockets *sock = (Sockets *)socket;
    int LenLTE = sizeof(sock->ServerLTE_RECEIVER);

    printf("\n\nLTE || Receive Socket: %d\n", sock->sockLTE_RECEIVER);
    RX_LTE = recvfrom(sock->sockLTE_RECEIVER, message_LTE, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE_RECEIVER, (unsigned int*)&LenLTE);
    Timestamp();

    // printf("LTE || LTE-Thread id = %ld\n", pthread_self());
    printf("LTE || Message from LTE received at: %s\n", curr_time);
    printf("LTE || Message is: %s\n\n", message_LTE);
    return message_LTE;
}



/* Function to receive WiFi packets */
void *receiveWiFi(void *socket) {
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->ServerWiFi_RECEIVER);

    printf("\n\nWiFi || Receive Socket: %d\n", sock->sockWiFi_RECEIVER);
    RX_WiFi = recvfrom(sock->sockWiFi_RECEIVER, message_WiFi, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi_RECEIVER, (unsigned int*)&LenWiFi);
    Timestamp();

    // printf("WiFi || WiFi-Thread id = %ld\n", pthread_self());
    printf("WiFi || Message from WiFi received at: %s \n", curr_time);
    printf("WiFi || Message is: %s\n\n", message_WiFi);

    return message_WiFi;    
}

void* receive_data() {
    Sockets *sock = (Sockets*)socket;
    void* msg = receiveWiFi(&sock);
    return msg;
}

/* -------- end ------------
------- RECEIVE AREA -------
--------------------------*/

/* -------------------------
------ TRANSMIT AREA -------
---------- start ---------*/

void Sockets_Transmitter(Sockets *sock, const char *IP_LTE, const char *IP_WiFi, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi) {
    /* Create socket receiver */
    sock->sockLTE_TRANSMITTER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi_TRANSMITTER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    printf("Sockets_Transmitter LTE (INSIDE): %d\n", sock->sockLTE_TRANSMITTER);
    printf("Sockets_Transmitter WiFi (INSIDE): %d\n", sock->sockWiFi_TRANSMITTER);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE_TRANSMITTER, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_TRANSMITTER, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Error checking */
    if (sock->sockLTE_TRANSMITTER == -1) {
        perror("Failed to create LTE transmitter socket");
        exit(0);
    }
    if (sock->sockWiFi_TRANSMITTER == -1) {
        perror("Failed to create WiFi transmitter socket");
        exit(0);
    }
    printf("Sockets_TRANSMITTER sucessfully created\n");

    /* Configure settings to communicate with remote UDP client for receiver */
    sock->ClientLTE_TRANSMITTER.sin_family = AF_INET;
    sock->ClientLTE_TRANSMITTER.sin_port = htons(PORT_LTE);
    sock->ClientLTE_TRANSMITTER.sin_addr.s_addr = inet_addr(IP_LTE);

    sock->ClientWiFi_TRANSMITTER.sin_family = AF_INET;
    sock->ClientWiFi_TRANSMITTER.sin_port = htons(PORT_WiFi);
    sock->ClientWiFi_TRANSMITTER.sin_addr.s_addr = inet_addr(IP_WiFi);
}


// Function to transmit GSV via LTE
void *transmitLTE(void *socket) {
    Sockets *sock = (Sockets *)socket;
    int LenLTE = sizeof(sock->sockLTE_TRANSMITTER);
    const char *GSV;
    const char *GSV_KEY = "GSV_KEY";
    GSV = shm_read(32, GSV_KEY);
    printf("\n\nLTE || Transmit Socket: %d\n", sock->sockLTE_TRANSMITTER);
    TX_LTE = sendto(sock->sockLTE_TRANSMITTER, GSV, BUFFER, 0, (struct sockaddr *)&sock->ClientLTE_TRANSMITTER, LenLTE);
    // printf("LTE-Thread id = %ld\n", pthread_self());
    printf("LTE || Shared memory object LTE: %s\n", GSV);
    printf("LTE || Message from LTE transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

// Function to transmit GSV via WiFi
void *transmitWiFi(void *socket) {
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->ClientWiFi_TRANSMITTER);
    const char *GSV;
    const char *GSV_KEY = "GSV_KEY";
    GSV = shm_read(32, GSV_KEY);
    printf("\n\nWiFi || Transmit Socket: %d\n", sock->sockWiFi_TRANSMITTER);
    TX_WiFi = sendto(sock->sockWiFi_TRANSMITTER, GSV, BUFFER, 0, (struct sockaddr *)&sock->ClientWiFi_TRANSMITTER, LenWiFi);
    // printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("WiFi || Shared memory object WiFi: %s\n", GSV);
    printf("WiFi || Message from WiFi transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}






// Function to transmit GSV via LTE
void* transmit_command_LTE(void *socket, char* message) {

    Sockets *sock = (Sockets *)socket;
    int LenLTE = sizeof(sock->sockLTE_TRANSMITTER);
    printf("\n\nLTE || Transmit Socket: %d\n", sock->sockLTE_TRANSMITTER);
    // char* lort;
    // strcpy(lort, message.c_str());
    TX_LTE = sendto(sock->sockLTE_TRANSMITTER,(const void*) message, BUFFER, 0, (struct sockaddr *)&sock->ClientLTE_TRANSMITTER, LenLTE);
    // printf("LTE-Thread id = %ld\n", pthread_self());
    printf("LTE || Sending Command to Actuator: %s\n", message);
    printf("LTE || Message transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

// Function to transmit GSV via WiFi
void* transmit_command_WiFi(void *socket, char* message) {
    
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->ClientWiFi_TRANSMITTER);
    const char *GSV;
    const char *GSV_KEY = "GSV_KEY";
    GSV = shm_read(32, GSV_KEY);
    printf("\n\nWiFi || Transmit Socket: %d\n", sock->sockWiFi_TRANSMITTER);
    TX_WiFi = sendto(sock->sockWiFi_TRANSMITTER, GSV, BUFFER, 0, (struct sockaddr *)&sock->ClientWiFi_TRANSMITTER, LenWiFi);
    // printf("WiFi-Thread id = %ld\n", pthread_self());
    printf("WiFi || Sending Command to Actuator: %s\n", GSV);
    printf("WiFi || Message transmitted at: %s\n\n", curr_time);
    pthread_exit(NULL);
}

// Function to transmit GSV via WiFi
void* transmit_command(void *socket, char* message) {
    cout << "============\n==== SEND ====\n============\nMessage passed to the general transmit commmand: \n" << message << endl;
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->ClientWiFi_TRANSMITTER);
    const char *GSV;
    const char *GSV_KEY = "GSV_KEY";
    GSV = shm_read(32, GSV_KEY);
    if(GSV == "0" || GSV == "1") {
        transmit_command_WiFi(&sock, message);
    }
    if(GSV == "0" || GSV == "2") {
        transmit_command_LTE(&sock, message);
    }
    return message;
}


/* -------- end ------------
------ TRANSMIT AREA -------
--------------------------*/