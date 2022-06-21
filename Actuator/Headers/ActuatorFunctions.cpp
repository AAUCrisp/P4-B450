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

//#include "hex.h"

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
char curr_time[128];
std::ofstream File;

/* --- Conversion from Integer to Hex-chars --- */
/*
string int_to_hex(int data, int back = 0) {
    stringstream stream;    // Create hex-conversion object

    stream << hex << data;    // Tell it to convert from int to hex-chars
    string result( stream.str() );      // Do the conversion
    cout << "Converted to Hex-chars is: " << result << endl;     // Print the result
*/

    /* -- Coversion back in same object, which already has the value -- */
  /*  if(back == 1) {
        int x;
        stream >> x;        // Write the coverted integer to the integer variable
        cout << "Converted back is: " << x << endl;
    }
    return result;
}*/

/* --- Conversion from Hex-chars to Integer --- */
int hex_to_int(string data) {
    stringstream stream;    // Create hex-conversion object
    int result;      // Create a variable for the converted integer
    stream << hex << data;    // Take the received hex-chars and covert to an integer
    stream >> result;        // Write the coverted integer to the integer variable
    //cout << "Converted back is: " << result << endl;

    return result;
}

/*
void logData(int *arr) {
    File.open("log.txt", std::ofstream::out | std::ofstream::app);
    File << "\n\n Movement on the x-axis:" << arr[0] << "\n Movement on the y-axis:" << arr[1];
    File.close();
}*/

char *Timestamp() {
    /* Timestamp format : [hh:mm:ss dd/mm/yy] */
    struct tm *timeinfo;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    timeinfo = localtime(&tv.tv_sec);

    sprintf(curr_time, "[%d:%d:%d.%03ld %d/%d/%d]", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tv.tv_usec / 1000, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    return curr_time;
}

/* This function updates the last coordinates, for next computation of finding movement over the X and Y axis */
void update_last_coordinate(int number1, int number2) {
    last_x_coordinate = number1;
    last_y_coordinate = number2;
}

/* This functions Parses the received message into a specified format. And finds the difference between the current and last coordinates. */
void processData(char msg[buffer]) {
    int movement_x;
    int movement_y;
    bytes_read = sscanf(msg, "%[^:%s]:%s:", tempx, tempy);  // Parses the received char array, into two seperate char arrays
    // cout << "Temp X is: " << tempx << endl;                 // prints out the char arrays containing x and y coordinates.
    // cout << "Temp Y is: " << tempy << endl;
    int x = hex_to_int(tempx);  // Converts the hex-chars of the x coordinate to an int
    int y = hex_to_int(tempy);  // Converts the hex-chars of the x coordinate to an int

    if (last_x_coordinate == 0 && last_y_coordinate == 0) {
        movement_x = x;
        movement_y = y;
        // printf("movement_x = x: %d\n", movement_x);
        // printf("movement_y = y: %d\n", movement_y);
    } else {
        movement_x = x - last_x_coordinate;
        movement_y = y - last_y_coordinate;
        // printf("movement_x = x - last_x: %d\n", movement_x);
        // printf("movement_y = y - last_y: %d\n", movement_y);
    }
    // printf("Movement_x: %d \nMovement_y: %d \n\n", movement_x, movement_y);
    if (movement_x == 0 && movement_y == 0) {
    } else {
        Timestamp();
        File.open("log.txt", std::ofstream::out | std::ofstream::app);
        File << "\n\n"
             << curr_time << "\nMovement on the x-axis:" << movement_x << " mm \nMovement on the y-axis:" << movement_y << " mm";
        File.close();
    }

    update_last_coordinate(x, y);
}
