#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
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

// Own Libraries
#ifndef VARIABLES
#define VARIABLES
    // General Settings
    static int both_tech = 0;      // Troubleshooting with both technologies
    static int monitor = 1;         // Disable/Enable Start of Signal Monitoring
    static int iter = 0;         // Disable/Enable Start of Signal Monitoring

#endif

/*
#ifndef SOCKET_FUNCTIONS
#define SOCKET_FUNCTIONS
#include "Headers/shm_write_read.h"
#endif

#ifndef SOCKET_FUNCTIONS
#define SOCKET_FUNCTIONS
#include "Headers/SocketFunctions.h"
#endif*/
