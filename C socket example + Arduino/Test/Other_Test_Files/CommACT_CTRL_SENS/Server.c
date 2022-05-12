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
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(){
    int sockLTE1 = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
    int sockWiFi1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    setsockopt(sockLTE1,SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
    setsockopt(sockWiFi1,SOL_SOCKET, SO_BINDTODEVICE, LTE, strlen(LTE));
}