#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
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

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "hex.cpp"

/* Variables used to create and bind sockets, receive from sockets, and error handle on sockets */
#define buffer 1024
char ActuatorBuffer[1024];
char feedback[1024];

/* Variables used to process data */
int bytes_read;
int x, y;
int last_x_coordinate;
int last_y_coordinate;
int movement_x;
int movement_y;

char tempx[buffer];
char tempy[buffer];

std::ofstream File;

/*
void logData(int *arr) {
    File.open("log.txt", std::ofstream::out | std::ofstream::app);
    File << "\n\n Movement on the x-axis:" << arr[0] << "\n Movement on the y-axis:" << arr[1];
    File.close();
}*/

/* This function updates the last coordinates, for next computation of finding movement over the X and Y axis */
void update_last_coordinate(int number1, int number2) {
    last_x_coordinate = number1;
    last_y_coordinate = number2;
}

/* This functions Parses the received message into a specified format. And finds the difference between the current and last coordinates. */
void processData(char msg[buffer]) {
    bytes_read = sscanf(msg, "%[^:%s]:%s:", tempx, tempy);  // Parses the received char array, into two seperate char arrays
    cout << "Temp X is: " << tempx << endl;                 // prints out the char arrays containing x and y coordinates.
    cout << "Temp Y is: " << tempy << endl;
    x = hex_to_int(tempx);  // Converts the hex-chars of the x coordinate to an int
    y = hex_to_int(tempy);  // Converts the hex-chars of the x coordinate to an int

    if (last_x_coordinate == 0 && last_y_coordinate == 0) {
        int movement_x = x;
        int movement_y = y;
        printf("movement_x = x: %d\n", movement_x);
        printf("movement_y = y: %d\n", movement_y);
    } else {
        int movement_x = x - last_x_coordinate;
        int movement_y = y - last_y_coordinate;
        printf("movement_x = x - last_x: %d\n", movement_x);
        printf("movement_y = y - last_y: %d\n", movement_y);
    }
    File.open("log.txt", std::ofstream::out | std::ofstream::app);
    File << "\n\n Movement on the x-axis:" << movement_x << "\n Movement on the y-axis:" << movement_y;
    File.close();

    update_last_coordinate(x, y);
}
