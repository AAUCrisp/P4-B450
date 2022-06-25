#ifndef SOCKETS_H
#define SOCKETS_H

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

    /* Execution timing variable */
    int packet_count_LTE;
    int packet_count_WiFi;
    int fail_count;
    long double Execution_Sum;
    int STOP;

} Sockets, ExeVar;

/* Function to create receiver socket */
void Sockets_Receiver(Sockets *sock, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi);

/* Function to create transmitter socket */
void Sockets_Transmitter(Sockets *sock, const char *IP_LTE, const char *IP_WiFi, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi);

/* Function to receive LTE packets */
void *receiveLTE(void *socket);

/* Function to receive WiFi packets */
void *receiveWiFi(void *socket);

/* Function to transmit LTE packets */
void *transmitLTE(void *socket);

/* Function to transmit WiFi packets */
// void *transmitWiFi(void *socket);
void *transmitWiFi(void *socket);

/* Function to generate timestamp */
char *Timestamp();

/* Function to generate random int */
int generate(int Min, int Max);

#endif