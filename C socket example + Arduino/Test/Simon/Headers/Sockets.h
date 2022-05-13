#ifndef SOCKETS
#define SOCKETS

/* Function declarations here: */

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

/* Function to bind sockets */
void Create_Bind_Sockets(Sockets *sock, uint PORT_LTE, uint PORT_WiFi, uint PORT_LTE_TRANS, uint PORT_WiFi_TRANS, const char *LTE, const char *WiFi);

/* Function to receive LTE packets */
void *receiveLTE(void* sockLTE);

/* Function to receive WiFi packets */
void *receiveWiFi(void* sockWiFi);

/* Function to transmit LTE packets */
void *transmitLTE(void* sockLTE_transmit);

/* Function to transmit WiFi packets */
void *transmitWiFi(void* sockWiFi_transmit);

/* Function to timestamp packets */
char *Timestamp();

/* Function to get RSSI */
int RSSI_VAL();

/* Function to get RSRP */
int RSRP_VAL();

#endif