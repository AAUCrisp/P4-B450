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
    // General Settings
    static int force_tech = 0;      // Troubleshooting with both technologies
    static int monitor = 1;         // Disable/Enable Start of Signal Monitoring

    // Grid Settings
    static int use_grid = 0;        // Usage of RAM Grid
    static int x_axis = 5000;       // Length of the x-axis of the grid
    static int y_axis = 5000;       // Length of the y-axis of the grid
    static int coordinates = x_axis*y_axis;     // Complete amount of coordinates
    static string* grid;
    
    // Command Coversion
    static int use_hex = 1;     // Coordinates as Hex-Chars

    // Prints
    static int message_only = 1;                // Print messages only
    static int troubleshooting_print = 0;       // Print everything for troubleshooting
    static int print_sen_in = 0;                // Print incoming Sensor related things
    static int print_act_out = 0;               // Print outgoing Actuator related things
    static int print_GSV = 0;                   // Print GSV related things

    // Signal Monitoring Arguments
    static int delay = 1;
    static char* GSV_arg_used = NULL;
    static char* GSV_arg_both = NULL;
    static char* GSV_arg_both_arg = NULL;
    static char* GSV_arg_print = NULL;
    static char* GSV_sleep_arg = NULL;
    static char* GSV_sleep_delay = NULL;

#endif
