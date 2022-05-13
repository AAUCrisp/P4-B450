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

#include "Sockets_TX_RX.h"
#include "shm_write_read.h"

void Bind_Sockets_Receiver(){
    /* Create socket receiver */
    sock->sockLTE = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sock->sockWiFi = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* Setting up socket options & specifying interface for receiver */
    setsockopt(sock->sockLTE, SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sock->sockWiFi, SOL_SOCKET, SO_BINDTODEVICE, WiFi, strlen(WiFi));

    /* Error checking */
    if (sock->sockLTE == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }
    if (sock->sockWiFi == -1) {
        perror("Failed to create sockLTE");
        exit(0);
    }

    /* Configure settings to communicate with remote UDP client for receiver */
    sock->ServerLTE.sin_family = AF_INET;
    sock->ServerLTE.sin_port = htons(PORT_LTE);
    sock->ServerLTE.sin_addr.s_addr = INADDR_ANY;

    sock->ServerWiFi.sin_family = AF_INET;
    sock->ServerWiFi.sin_port = htons(PORT_WiFi);
    sock->ServerWiFi.sin_addr.s_addr = INADDR_ANY;

    /* Bind to socket */
    bindLTE = bind(sock->sockLTE, (struct sockaddr *)&sock->ServerLTE, sizeof(struct sockaddr));
    bindWiFi = bind(sock->sockWiFi, (struct sockaddr *)&sock->ServerWiFi, sizeof(struct sockaddr));

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
