#ifndef LIBRARIES
#define LIBRARIES
#include "../Libraries.cpp"  // File with all our includes
#endif

/*
#ifndef SHM_READ
#define SHM_READ
#include "shm_read.cpp"
#endif
*/

#include "shm_read_write.h"

#ifndef SOCKETS_NET
#define SOCKETS_NET
#include "Sockets.h"
#endif
#include <fstream>

std::ofstream File;

/* -- Trouble Shooting Setup -- */

/* -------------------------
-------- SETUP AREA --------
---------- start ---------*/

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

/* Define buffers & PORT number */
#define BUFFER 1024
char message_LTE[BUFFER];
char message_WiFi[BUFFER];
char *receive_LTE;
char *receive_WiFi;
char curr_time[128];
char msg_time[128];
char send_tech[128];
char *send_time;
int sensor_int;

/* Misc */
int bindLTE, bindWiFi;
int TX_LTE, TX_WiFi;

/* File descriptor */
FILE *fp1;
FILE *fp2;

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
    /* Time struct for socket timeout LTE */
    struct timeval tv2;
    tv2.tv_sec = 5;
    tv2.tv_usec = 0;

    /* Time struct for socket timeout WiFi */
    struct timeval tv3;
    tv2.tv_sec = 5;
    tv2.tv_usec = 0;

    /* Create socket receiver */
    sock->sockLTE_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi_RECEIVER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_RECEIVER, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));
    setsockopt(sock->sockLTE_RECEIVER, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));
    setsockopt(sock->sockWiFi_RECEIVER, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));

    /* Error checking */
    if (sock->sockLTE_RECEIVER == -1) {
        perror("Sensor Connection || Failed to create LTE Socket");
        exit(0);
    }
    if (sock->sockWiFi_RECEIVER == -1) {
        perror("Sensor Connection || Failed to create WiFi Socket");
        exit(0);
    }
    printf("  Sensor Connection || LTE & WiFi Receiving Sockets sucessfully created\n");

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
        perror("Sensor Connection || Failed to bind LTE socket");
        exit(0);
    }
    if (bindWiFi == -1) {
        perror("Sensor Connection || Failed to bind WiFi socket");
        exit(0);
    }
    printf("  Sensor Connection || LTE & WiFi Receiving Sockets sucessfully bound\n");
}

/* Function to receive random integer via LTE */
char lolcountLTE[1024];
void *receiveLTE(void *socket) {
    Sockets *sock = (Sockets *)socket;
    unsigned int LenLTE = sizeof(sock->ServerLTE_RECEIVER);

    if (print_sen_in == 1) {
        printf("\n\n  Incoming || LTE (Sensor) || Receive Socket: %d\n", sock->sockLTE_RECEIVER);
    }

    sock->RX_LTE = recvfrom(sock->sockLTE_RECEIVER, message_LTE, BUFFER, 0, (struct sockaddr *)&sock->ServerLTE_RECEIVER, &LenLTE);
    if (sock->RX_LTE == -1) {
        return 0;
    }
    Timestamp();
    sscanf(message, "%[^:]", lolcountLTE);
    sock->packet_count_LTE = atoi(lolcountLTE);
    printf("Packets LTE: %d\n", sock->packet_count_LTE);

    // sock->packet_count_LTE++;

    /* Open logging file */
    fp1 = fopen("Logs/log.txt", "a+");
    fprintf(fp1, "%s %s %s %s\n", message_LTE, "LTE", " : Received at ", curr_time);
    fclose(fp1);

    printf("\n\n  Incoming || LTE (Sensor) || Message is: %s\n", message_LTE);
    printf("  Incoming || LTE (Sensor) || Message received at: %s\n\n", curr_time);
    if (print_sen_in == 1 || message_only == 1) {
    }
    sscanf((const char *)message_LTE, "%d %[^\n]", &sensor_int, msg_time);

    return message_LTE;
}

/* Function to receive random integer via WiFi */
char lolcountWiFi[1024];
void *receiveWiFi(void *socket) {
    Sockets *sock = (Sockets *)socket;
    unsigned int LenWiFi = sizeof(sock->ServerWiFi_RECEIVER);

    if (print_sen_in == 1) {
        printf("\n\n  Incoming || WiFi (Sensor) || Receive Socket: %d\n", sock->sockWiFi_RECEIVER);
    }

    sock->RX_WiFi = recvfrom(sock->sockWiFi_RECEIVER, message_WiFi, BUFFER, 0, (struct sockaddr *)&sock->ServerWiFi_RECEIVER, &LenWiFi);
    if (sock->RX_WiFi == -1) {
        return 0;
    }
    Timestamp();
    sscanf(message2, "%[^:]", lolcountWiFi);
    sock->packet_count_WiFi = atoi(lolcountWiFi);
    printf("Packets WiFi: %d\n", sock->packet_count_WiFi);
    // sock->packet_count_WiFi++;

    /* Open logging file */
    fp2 = fopen("Logs/log.txt", "a+");
    fprintf(fp2, "%s %s %s%s\n", message_WiFi, "WiFi", ": Received at ", curr_time);
    fclose(fp2);

    printf("\n\n  Incoming || WiFi (Sensor) || Message is: %s\n", message_WiFi);
    printf("  Incoming || WiFi (Sensor) || Message received at: %s \n\n", curr_time);
    if (print_sen_in == 1 || message_only == 1) {
    }
    sscanf((const char *)message_WiFi, "%d %[^\n]", &sensor_int, msg_time);

    return message_WiFi;
}

void *receive_data() {
    Sockets *sock = (Sockets *)socket;
    void *msg = receiveWiFi(&sock);
    return msg;
}

/* -------- end ------------
------- RECEIVE AREA -------
--------------------------*/

/* -------------------------
------ TRANSMIT AREA -------
---------- start ---------*/

void Sockets_GSV(Sockets *sock, const char *IP_LTE, const char *IP_WiFi, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi) {
    /* Create socket receiver */
    sock->sockLTE_TRANSMITTER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi_TRANSMITTER = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE_TRANSMITTER, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi_TRANSMITTER, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Error checking */
    if (sock->sockLTE_TRANSMITTER == -1) {
        perror("GSV Connection || Failed to create GSV LTE transmitter socket");
        exit(0);
    }
    if (sock->sockWiFi_TRANSMITTER == -1) {
        perror("GSV Connection || Failed to create GSV WiFi transmitter socket");
        exit(0);
    }
    printf("  GSV Connection || LTE & WiFi Transmit Sockets sucessfully created\n");

    /* Configure settings to communicate with remote UDP client for receiver */
    sock->ClientLTE_TRANSMITTER.sin_family = AF_INET;
    sock->ClientLTE_TRANSMITTER.sin_port = htons(PORT_LTE);
    sock->ClientLTE_TRANSMITTER.sin_addr.s_addr = inet_addr(IP_LTE);
    if (print_GSV == 1) {
        cout << "  GSV || LTE IP: " << IP_LTE << endl;
    }

    sock->ClientWiFi_TRANSMITTER.sin_family = AF_INET;
    sock->ClientWiFi_TRANSMITTER.sin_port = htons(PORT_WiFi);
    sock->ClientWiFi_TRANSMITTER.sin_addr.s_addr = inet_addr(IP_WiFi);
    if (print_GSV == 1) {
        cout << "  GSV || WiFi IP: " << IP_WiFi << endl;
    }
}

void Sockets_Actuator(Sockets *sock, const char *IP_LTE, const char *IP_WiFi, uint PORT_LTE, uint PORT_WiFi, const char *LTE, const char *WiFi) {
    /* Time struct for socket timeout */
    struct timeval tv2;
    tv2.tv_sec = 0;
    tv2.tv_usec = 500000;

    /* Create socket receiver */
    sock->act_LTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->act_WiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->act_LTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->act_WiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Setting up socket timeout */
    // setsockopt(sock->act_LTE, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));
    // setsockopt(sock->act_WiFi, SOL_SOCKET, SO_RCVTIMEO, &tv2, sizeof(tv2));

    /* Error checking */
    if (sock->act_LTE == -1) {
        perror("Actuator Connection || Failed to create LTE transmitter socket");
        exit(0);
    }
    if (sock->act_WiFi == -1) {
        perror("Actuator Connection || Failed to create WiFi transmitter socket");
        exit(0);
    }
    printf("  Actuator Connection || LTE & WiFi Transmit Sockets sucessfully created\n");

    /* Configure settings to communicate with remote UDP client for receiver */
    sock->Client_act_LTE.sin_family = AF_INET;
    sock->Client_act_LTE.sin_port = htons(PORT_LTE);
    sock->Client_act_LTE.sin_addr.s_addr = inet_addr(IP_LTE);

    sock->Client_act_WiFi.sin_family = AF_INET;
    sock->Client_act_WiFi.sin_port = htons(PORT_WiFi);
    sock->Client_act_WiFi.sin_addr.s_addr = inet_addr(IP_WiFi);
}

// Function to transmit GSV via LTE
void transmit_GSV_LTE(void *socket, char *gsv) {
    Sockets *sock = (Sockets *)socket;
    int LenLTE = sizeof(sock->ClientLTE_TRANSMITTER);

    if (print_GSV == 1) {
        printf("\n\n  GSV || LTE || Transmit Socket: %d\n", sock->sockLTE_TRANSMITTER);
    }

    TX_LTE = sendto(sock->sockLTE_TRANSMITTER, gsv, BUFFER, 0, (struct sockaddr *)&sock->ClientLTE_TRANSMITTER, LenLTE);

    if (print_GSV == 1) {
        printf("  GSV || LTE || Technology Variable is: %s\n", gsv);
        printf("  GSV || LTE || GSV transmitted at: %s\n\n", curr_time);
    }
}

// Function to transmit GSV via WiFi
void transmit_GSV_WiFi(void *socket, char *gsv) {
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->ClientWiFi_TRANSMITTER);

    if (print_GSV == 1) {
        printf("\n\n  GSV || WiFi || Transmit Socket: %d\n", sock->sockWiFi_TRANSMITTER);
    }

    TX_WiFi = sendto(sock->sockWiFi_TRANSMITTER, gsv, BUFFER, 0, (struct sockaddr *)&sock->ClientWiFi_TRANSMITTER, LenWiFi);

    if (print_GSV == 1) {
        printf("  GSV || WiFi || Technology Variable is: %s\n", gsv);
        printf("  GSV || WiFi || GSV transmitted at: %s\n\n", curr_time);
    }
}

// Function to transmit command via LTE
void *transmit_command_LTE(void *socket, char *message) {
    Sockets *sock = (Sockets *)socket;
    int LenLTE = sizeof(sock->Client_act_LTE);

    // printf("\n\n  Sending || LTE (Actuator) || Actuator Socket: %d\n", sock->act_LTE);

    if (print_act_out == 1) {
    }

    TX_LTE = sendto(sock->act_LTE, message, BUFFER, 0, (struct sockaddr *)&sock->Client_act_LTE, LenLTE);
    send_time = Timestamp();

    // printf("  Sending || LTE (Actuator) || Sending Command to Actuator: %s %s\n", message, send_time);
    //  printf("  Sending || LTE (Actuator) || Message transmitted at: %s\n\n", send_time);
    //  printf("I am sending via LTE\n");

    if (print_act_out == 1 || message_only == 1) {
    }

    return 0;
}

// Function to transmit command via WiFi
void *transmit_command_WiFi(void *socket, char *message) {
    Sockets *sock = (Sockets *)socket;
    int LenWiFi = sizeof(sock->Client_act_WiFi);

    // printf("\n\n  Sending || WiFi (Actuator) || Actuator Socket: %d\n", sock->act_WiFi);

    if (print_act_out == 1) {
    }

    TX_WiFi = sendto(sock->act_WiFi, message, BUFFER, 0, (struct sockaddr *)&sock->Client_act_WiFi, LenWiFi);
    send_time = Timestamp();
    // printf("  Sending || WiFi (Actuator) || Sending Command to Actuator: %s %s\n", message, send_time);
    //  printf("  Sending || WiFi (Actuator) || Message transmitted at: %s\n\n", send_time);
    //  printf("I am sending via WiFi\n");

    if (print_act_out == 1 || message_only == 1) {
    }

    return 0;
}

// Function to check GSV and transfer via chosen technologies
void *transmit_command(void *socket, char *message, int gsv) {
    Sockets *sock = (Sockets *)socket;

    if (print_act_out == 1) {
        // cout << "  ======================\n  ==== SEND COMMAND ====\n  ======= entry ========\n" << endl;
        // printf("\n\n  Sending || WiFi (Actuator) || Sockets in Transmit Command: %d\n", testsock->act_WiFi);
        // printf("\n  Sending || LTE (Actuator) || Sockets in Transmit Command: %d\n", testsock->act_LTE);
        // cout << "\n  Sending || Mutual Transmit Function || Message passed to function: \n"<< message << endl;
    }
    // int LenWiFi = sizeof(sock->Client_act_WiFi); Burde ik stå her

    // cout << "  Sending || Global Signal Variable is: " << gsv << endl;
    if (print_act_out == 1) {
    }

    if ((gsv == 0) || (gsv == 1)) {
        if (print_act_out == 1) {
        }
        // cout << "  Sending || Transfer command via WiFi" << endl;
        transmit_command_WiFi(sock, message);
    }
    if ((gsv == 0) || (gsv == 2)) {
        if (print_act_out == 1) {
        }
        // cout << "  Sending || Transfer command via LTE" << endl;
        // transmit_command_LTE(sock, message); // Original function
    }
    if (print_act_out == 1) {
        cout << "\n  ======== end ==========\n  ==== SEND COMMAND ====\n  ======================\n"
             << endl;
    }

    return 0;
}

/**
 * @brief Test function of the same function as transmit_command( );
 * @param socket
 * @param message
 * @param gsv
 * @return void*
 */
void *transmit_command_test(void *socket, char *message, int gsv) {
    Sockets *sock = (Sockets *)socket;

    if (print_act_out == 1) {
        // cout << "  ======================\n  ==== SEND COMMAND ====\n  ======= entry ========\n" << endl;
        // printf("\n\n  Sending || WiFi (Actuator) || Sockets in Transmit Command: %d\n", testsock->act_WiFi);
        // printf("\n  Sending || LTE (Actuator) || Sockets in Transmit Command: %d\n", testsock->act_LTE);
        // cout << "\n  Sending || Mutual Transmit Function || Message passed to function: \n"<< message << endl;
    }
    // int LenWiFi = sizeof(sock->Client_act_WiFi); Burde ik stå her

    // cout << "  Sending || Global Signal Variable is: " << gsv << endl;
    if (print_act_out == 1) {
    }

    if ((gsv == 0) || (gsv == 1)) {
        if (print_act_out == 1) {
        }
        // cout << "  Sending || Transfer command via WiFi" << endl;
        // transmit_command_WiFi(sock, message);
    }
    if ((gsv == 0) || (gsv == 2)) {
        if (print_act_out == 1) {
        }
        // cout << "  Sending || Transfer command via LTE" << endl;
        transmit_command_LTE(sock, message);
    }
    if (print_act_out == 1) {
        cout << "\n  ======== end ==========\n  ==== SEND COMMAND ====\n  ======================\n"
             << endl;
    }

    return 0;
}

/* -------- end ------------
------ TRANSMIT AREA -------
--------------------------*/