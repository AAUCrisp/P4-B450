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

#include "Headers/SocketFunctions.h"
#include "Headers/shm_write_read.h"

#define BUFFER 64

int main(int argc, char* argv[]) {
    int both_tech = 0;
    int monitor = 1;

    printf("\n==================\nSensor Program Started\n==================\n\n");

    if (argc > 1) {  // If the program is run with arguments
        printf("\nArgument(s) accepted.\n");

        int aug1 = atoi(argv[1]);
        int aug2 = atoi(argv[2]);

        printf("\nBefore Monitor Argument\n");
        if (aug1 == 0) {  // For disabling GSV Update
            printf("\n=====  Monitoring Disabled =====\n");
            monitor = 0;
        }

        printf("\nBefore Both Argument\n");
        if (aug2 == 1) {  // For enabling using both LTE and WiFi.
            printf("\n=====  Forced Both LTE & WiFi =====\n");
            both_tech = 1;
        }
    } else {
        printf("\nNo arguments inserted, running staticly.\n");
    }

    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE_TRANSMITTER = 9000;
    uint PORT_WiFi_TRANSMITTER = 9001;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";
    const char* IP_LTE = "10.20.0.16";  // Default: IP of Control Unit
    // const char* IP_LTE = "10.20.0.13";      // IP of Actuator
    // const char* IP_LTE = "10.20.0.10";      // IP of Sensor
    const char* IP_WiFi = "192.168.1.136";  // Default: IP of Control Unit
    // const char* IP_WiFi = "192.168.1.143";  // IP of Actuator
    // const char* IP_WiFi = "192.168.1.160";  // IP of Sensor

    /* Misc */
    pthread_t T1, T2;
    char* curr_time;

    /* Execution time variables */
    int iter = 1000;
    long double Execution_Time[iter];
    long double Execution_Sum;
    long double Execution_Average;
    clock_t Clock_Start;
    clock_t Clock_End;

    /* Create sockets */
    Sockets sock;
    Sockets_Transmitter(&sock, IP_LTE, IP_WiFi, PORT_LTE_TRANSMITTER, PORT_WiFi_TRANSMITTER, LTE, WiFi);
    printf("sockLTE_TRANSMITTER (OUTSIDE): %d\n", sock.sockLTE_TRANSMITTER);
    printf("sockWiFi_TRANSMITTER (OUTSIDE): %d\n\n", sock.sockWiFi_TRANSMITTER);

    /* Shared memory object variables */
    const char* GSV_KEY = "GSV_KEY";
    const char* msg;

    char buffer[BUFFER];

    int GSV = 0;
    int B = 0;
    int W = 1;
    int L = 2;

    /* Create child process */
    pid_t sensor_monitor;     // Prepare the process ID for monitoring
    sensor_monitor = fork();  // Starts new process

    if (sensor_monitor == 0) {
        printf("Parent process ID: %d \n", getppid());
        printf("Sensor monitoring process ID is: %d \n", getpid());
        char path[] = "./SensorMonitoring";
        char* args[] = {"./SensorMonitoring", NULL};
        /*if (monitor == 1) {
            execv(path, args);
        }*/
    } else {
        // while (1) {
        for (int i = 0; i < iter; i++) {
            //usleep(1000);
            //msg = shm_read(32, GSV_KEY);
            //GSV = atoi(msg);
            //printf("\nSensor || GSV from shared memory: %s\n", msg);

            // printf("\nGSV converted: %d\n", GSV);
            /*if (monitor == 1) {
            }*/
            Clock_Start = clock();
            sprintf(buffer, "%d", generate(1, 25000000));
            // printf("\nSensor || After Random Int Generation\n");
            //usleep(2000);

            /*if (both_tech == 1) {
                printf("\nSensor || Troubleshooting for Both Technologies\n");
                GSV = 0;  // Troubleshooting for both
            }*/

            // printf("Sensor || Before Transmitting\n");
            if (GSV == B || GSV == L) {
                transmitLTE(&sock, (char*)buffer);
            }

            if (GSV == B || GSV == W) {
                transmitWiFi(&sock, (char*)buffer);
            }
            Clock_End = clock();
            Execution_Time[i] += (double)(Clock_End - Clock_Start) / CLOCKS_PER_SEC;
            ;
            printf("Execution_Time[%d]: %Lf\n", i, Execution_Time[i]);
            Execution_Sum += Execution_Time[i];
            printf("Execution_Sum = %Lf\n", Execution_Sum);
            // printf("Execution time: %f ms \n", Execution_Time);
            // sleep(3);
            usleep(100000);
        }
        Execution_Average = Execution_Sum / iter;
        printf("Execution average: %Lf\n", Execution_Average);
        //}
    }
}
