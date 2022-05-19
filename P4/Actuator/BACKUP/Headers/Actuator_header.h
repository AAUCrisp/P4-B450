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
#include <netdb.h>
#include <string>
#include <fstream>
#include <sstream>
#include "hex.cpp"
#include <iostream>


#define buffer 1024
/* Variables used to create and bind sockets, receive from sockets, and error handle on sockets*/
struct addrinfo ServerLTE, *res1;   
struct addrinfo ServerWiFi, *res;  
struct sockaddr_storage remote_addr;
socklen_t fromlen;
int rv, rl;
int sockLTE, sockWiFi;
int bindLTE, bindWiFi;
int lenLTE = sizeof(ServerLTE);
int lenWiFi = sizeof(ServerWiFi);
int rc_LTE, rc_WiFi;
pthread_t T1, T2;

const char *LTE = "wwan0";
const char *WiFi = "wlan0";
char s[INET_ADDRSTRLEN];
char ActuatorBuffer[1024];
char feedback[1024];



/* Variables used by the shared memory functions: shm_read and shm_write*/
char *received_message;
const char* GSV_KEY = "GSV_KEY";



/*Variables used to process data*/
int bytes_read;
int x, y;
int last_x_coordinate;
int last_y_coordinate;
int movement_x;
int movement_y;

char tempx [buffer];
char tempy [buffer];

std::ofstream File;


/*This function updates the last coordinates, for next computation of finding movement over the X and Y axis*/
void update_last_coordinate(int number1, int number2)
{
    last_x_coordinate = number1;
    last_y_coordinate = number2;
}


/* This functions Parses the received message into a specified format. And finds the difference between the current and last coordinates.*/
int * process_Data(char msg[buffer]) {
    int * result = (int *)malloc(sizeof(int) * 2);          //creates an int pointer to the array containing movement over the x and y axis. 

    bytes_read = sscanf(msg, "%[^:%s]:%s:", tempx, tempy); // Parses the received char array, into two seperate char arrays
    cout << "Temp X is: " << tempx << endl;                // prints out the char arrays containing x and y coordinates. 
    cout << "Temp Y is: " << tempy << endl;
    x = hex_to_int(tempx);                                 // Converts the hex-chars of the x coordinate to an int
    y = hex_to_int(tempy);                                 // Converts the hex-chars of the x coordinate to an int


    if (last_x_coordinate && last_y_coordinate == 0)
    {
        int movement_x = x;
        int movement_y = y;
    } else {
        int movement_x = x - last_x_coordinate;
        int movement_y = y - last_y_coordinate;
    }

    result[0] = movement_x;
    result[1] = movement_y;
    update_last_coordinate(x, y);
    
    return result;                                      // returns the array containing movement over the x and y axis. 
}


