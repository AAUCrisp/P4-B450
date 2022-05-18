#include "Headers/shm_write_read.h"
#include "Headers/data_header.h"
#include <iostream>

const char *received_message;
const char *GSV_KEY = "GSV_KEY";
char msg[1024];


void logData1(int *arr) 
{
    File.open("log.txt", std::ofstream::out | std::ofstream::app);
    //std::ostringstream out;
    //out << "\n\n movement on the x-axi:" << arr[0] << "\n movement on the y-axis:" << arr[1];
    //sprintf(temp, "Movement on x and y axis:  x = %d, y = %d \n \n", arr[0], arr[1]);
    //fwrite(temp, sizeof(char), strlen(temp), out);
    File << "\n\n movement on the x-axi:" << arr[0] << "\n movement on the y-axis:" << arr[1];
    File.close();
}
/*
void logData2(int *arr)
{
    WiFi_File.open("WiFi-log.txt", std::ofstream::out | std::ofstream::app);
    //std::ostringstream movement;
    //movement << "\n\n Movement on the x-axis:" << arr[0] << "\n movement on the y-axis:" << arr[1];
    WiFi_File << "\n\n Movement on the x-axis:" << arr[0] << "\n movement on the y-axis:" << arr[1];
    WiFi_File.close();
}
*/

int main()
{
    printf("==================\nChild Process Started\n==================\n\n");
    while(1)
    {
    
    received_message = shm_read(1024, GSV_KEY);

    snprintf(msg, sizeof(msg), "%s" , received_message);

    int* ra = process_Data(msg);
    logData1(ra);
    usleep(100000);

    }
    
}

