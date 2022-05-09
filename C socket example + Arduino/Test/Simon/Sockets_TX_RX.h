#ifndef SOCKETS_TX_RX
#define SOCKETS_TX_RX

/* Function declarations here: */

struct Sockets {
        int sockLTE;
        int sockWiFi;
        struct sockaddr_in ServerLTE;
        struct sockaddr_in ServerWiFi;
    };

/* Function to bind sockets */
void Create_Bind_Sockets(int *sockLTE, int *sockWiFi, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi);

/* Function to receive LTE packets */
void *receiveLTE(void* sockLTE);

/* Function to receive WiFi packets */
void *receiveWiFi(void* sockWiFi);

/* Function to transmit LTE packets */
void *transmitLTE(int data);

/* Function to transmit WiFi packets */
void *transmitWiFi(int data);

/* Function to timestamp packets */
char *Timestamp();

#endif