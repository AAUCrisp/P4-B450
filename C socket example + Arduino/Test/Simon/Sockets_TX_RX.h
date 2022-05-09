#ifndef SOCKETS_TX_RX
#define SOCKETS_TX_RX

/* Function declarations here: */


/* Function to bind sockets */
void Create_Bind_Sockets(uint PORT_LTE, uint PORT_WiFi);

/* Function to receive LTE packets */
void *receiveLTE(char message, size_t buffer);

/* Function to receive WiFi packets */
void *receiveWiFi(char message, size_t buffer);

/* Function to transmit LTE packets */
void *transmitLTE(char message, size_t buffer);

/* Function to transmit WiFi packets */
void *transmitWiFi(int GSV, size_t buffer);

/* Function to timestamp packets */
char *Timestamp();

#endif