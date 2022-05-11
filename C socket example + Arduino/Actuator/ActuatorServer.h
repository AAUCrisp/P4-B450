#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>



//define various socket variables, buffers and thread variables. 



void Create_Bind_Sockets(uint);

void *ReceiveLTE();

void ReceiveWiFi();

void *TransmitLTE();

void TransmitWiFi();


