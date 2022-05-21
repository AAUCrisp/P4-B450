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


#ifndef VARIABLES
#define VARIABLES
    // Control Unit
    static int monitor = 0;        // Disable/Enable Start of Signal Monitoring
    static int troubleshooting_print = 0;
    static int use_grid = 0;

    // Converter
    static int x_axis = 5000;      // Length of the x-axis of the grid
    static int y_axis = 5000;      // Length of the y-axis of the grid
    static int coordinates = x_axis*y_axis;
    static int use_hex = 1;

    // Functions
    static int print_unimportant = 0;

    // Sockets
    static int message_only = 1;        // Print messages only
    static int print_sen_in = 0;       // Print incoming Sensor related things
    static int print_act_out = 0;      // Print outgoing Actuator related things
    static int print_GSV = 0;          // Print GSV related things
    static int force_both = 0;         // Troubleshooting with both technologies

    // Signal Monitoring
    static int print = 0;      // Enable/Disable prints for troubleshooting
    static int only_message = 1;   // Only print Selection
    static int both_tech = 1;      // Forces it to use both LTE & WiFi


#endif

// Own Libraries
// #include "Headers/Sockets.h"
// Sockets sock;
// #ifndef SHM_LIB 
// #define SHM_LIB
// #include "Headers/shm_write.cpp"
// #include "Headers/shm_read.cpp"
// #include "Headers/shm_write_read.h"
// #endif

