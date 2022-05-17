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
} Sockets;

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
void *transmitWiFi(void *socket);

char *Timestamp();

#endif