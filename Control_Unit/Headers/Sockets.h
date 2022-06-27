#ifndef SOCKETS
#define SOCKETS

/* Function declarations here: */
#ifndef SOCKET_STRUCT
#define SOCKET_STRUCT

typedef struct _sockets {
    /* Receiver sockets*/
    int sockLTE_RECEIVER;
    int sockWiFi_RECEIVER;
    struct sockaddr_in ServerLTE_RECEIVER;
    struct sockaddr_in ServerWiFi_RECEIVER;
    int RX_LTE;
    int RX_WiFi;

    /* Transmitter sockets*/
    int sockLTE_TRANSMITTER;
    int sockWiFi_TRANSMITTER;
    struct sockaddr_in ClientLTE_TRANSMITTER;
    struct sockaddr_in ClientWiFi_TRANSMITTER;

    /* Actuator sockets*/
    int act_LTE;
    int act_WiFi;
    struct sockaddr_in Client_act_LTE;
    struct sockaddr_in Client_act_WiFi;

    /* Execution timing variable */
    int packet_count_LTE;
    int packet_count_WiFi;
    int fail_count_LTE;
    int fail_count_WiFi;
    long double Execution_Sum_LTE;
    long double Execution_Sum_WiFi;
    int STOP_LTE;
    int STOP_WiFi;
    int packet_sent_LTE;
    int packet_sent_WiFi;

} Sockets;

typedef struct _time_struct {
    int hour;
    int minute;
    int second;
    int millis;
} time_struct;

#endif

/* Function to create receiver socket */
void Sockets_Receiver(Sockets *sock, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi);

/* Function to create transmitter socket */
void Sockets_GSV(Sockets *sock, const char *IP_LTE, const char *IP_WiFi, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi);

void Sockets_Actuator(Sockets *sock, const char *IP_LTE, const char *IP_WiFi, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi);

/* ------------------------------------------------------------------------------------ */

/* ========== RECEIVE INT SENSOR ==========*/
/* Function to receive LTE packets */
void *receiveLTE(void *socket);

/* Function to receive WiFi packets */
void *receiveWiFi(void *socket);
/* ========== RECEIVE INT SENSOR ==========*/



/* ------------------------------------------------------------------------------------ */



/* ========== TRANSMIT GSV SENSOR ==========*/
/* Function to transmit LTE packets */
void transmit_GSV_LTE(void *socket, char *gsv);

/* Function to transmit WiFi packets */
void transmit_GSV_WiFi(void *socket, char *gsv);
/* ========== TRANSMIT GSV SENSOR ==========*/



/* ------------------------------------------------------------------------------------ */



/* ========== TRANSMIT COMMANDS ACTUATOR ==========*/
/* Function to transmit LTE packets */
void *transmit_command_LTE(void *socket, char *message);

/* Function to transmit WiFi packets */
void *transmit_command_WiFi(void *socket, char *message);

/* Function to transmit WiFi & LTE packets */
void *transmit_command(void *socket, char *message, int gsv);
/* ========== TRANSMIT COMMANDS ACTUATOR ==========*/



/* ------------------------------------------------------------------------------------ */


/* Function to receive WiFi packets */
void *receive_data();

/* Function to timestamp packets */
char *Timestamp();

/* Function to get RSSI */
int RSSI_VAL();

/* Function to get RSRP */
int RSRP_VAL();

#endif