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

#include<iomanip>
#include <cstdio>
#include<iostream>
#include <cstring>
using namespace std;

// #ifndef VARIABLES
// #define VARIABLES
// /* -- Our General Variables -- */
// int x_axis = 5000;      // Length of the x-axis of the grid
// int y_axis = 5000;      // Length of the y-axis of the grid
// int coordinates = x_axis*y_axis;

// #endif

// Own Libraries
// #include "Headers/Sockets.h"
// Sockets sock;
// #ifndef SHM_LIB 
// #define SHM_LIB
// #include "Headers/shm_write.cpp"
// #include "Headers/shm_read.cpp"
// #include "Headers/shm_write_read.h"
// #endif

