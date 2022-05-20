#include "Headers/shm_write_read.h"
#include "Headers/Actuator_header.h"


char msg[1024];


void logData1(int *arr) 
{
    File.open("log.txt", std::ofstream::out | std::ofstream::app);
    File << "\n\n movement on the x-axi:" << arr[0] << "\n movement on the y-axis:" << arr[1];
    File.close();
}



int main()
{
    printf("==================\nChild Process Started\n==================\n\n");
    while(1)
    {
    
    received_message = shm_read(1024, GSV_KEY); // READS DATA FROM SHARED MEMORY

    snprintf(msg, sizeof(msg), "%s" , received_message); // PRINTS THE DATA FROM SHARED MEMORY TO THE CHAR ARRAY MSG

    int* ra = process_Data(msg); //PROCESS DATA RECEIVED DATA
    logData1(ra);
    usleep(100000);
    }
    
}
