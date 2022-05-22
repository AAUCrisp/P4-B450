// #include <arpa/inet.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <math.h>
// #include <netinet/in.h>
// #include <pthread.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/ipc.h>  //IPC thing
// #include <sys/mman.h>
// #include <sys/sem.h>
// #include <sys/shm.h>  //SHM thing
// #include <sys/socket.h>
// #include <sys/stat.h>
// #include <sys/time.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <time.h>
// #include <unistd.h>

#ifndef LIBRARIES
#define LIBRARIES
#include "Libraries.c"  // File with all our includes
#endif
#ifndef FUNCTIONS
#define FUNCTIONS
#include "Headers/Functions.c"
#endif
// #include "Headers/Functions.c"
#include "Headers/SocketFunctions.h"
#include "Headers/shm_write_read.h"

#define BUFFER 64

int main(int argc, char* argv[]) {
    // int both_tech = 0;
    // int monitor = 1;
    // int iter = 10000;

    printf("\n==================\nSensor Program Started\n==================\n\n");

    // if (argc <= 1) {}

    if (argc > 1) {  // If the program is run with arguments
        printf("\nArgument(s) accepted.\n");

        Argument_Setup(argc, argv);

        // int aug1 = atoi(argv[1]);
        // int aug2 = atoi(argv[2]);

        // printf("\nBefore Monitor Argument\n");
        // if (aug1 == 0) {  // For disabling GSV Update
        //     printf("\n=====  Monitoring Disabled =====\n");
        //     monitor = 0;
        // }

        // printf("\nBefore Both Argument\n");
        // if (aug2 == 1) {  // For enabling using both LTE and WiFi.
        //     printf("\n=====  Forced Both LTE & WiFi =====\n");
        //     both_tech = 1;
        // }
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
    FILE* file;
    char* curr_time;

    /* Execution time variables */
    int fail_count = 0;
    long double Execution_Time[iter];
    long double Real_Execution_Time[iter];
    long double Execution_Temp = 0;
    long double Execution_Sum = 0;
    long double Real_Execution_Sum = 0;
    long double Execution_Average = 0;
    clock_t Time_Started;
    clock_t Time_Ended;
    clock_t Clock_Start;
    clock_t Clock_End;

    struct timespec begin, end;
    unsigned long seconds = 0;
    unsigned long nanoseconds = 0;
    double elapsed = 0;

    /* Create sockets */
    Sockets sock;
    Sockets_Transmitter(&sock, IP_LTE, IP_WiFi, PORT_LTE_TRANSMITTER, PORT_WiFi_TRANSMITTER, LTE, WiFi);
    printf("sockLTE_TRANSMITTER (OUTSIDE): %d\n", sock.sockLTE_TRANSMITTER);
    printf("sockWiFi_TRANSMITTER (OUTSIDE): %d\n\n", sock.sockWiFi_TRANSMITTER);

    /* Shared memory object variables */
    const char* GSV_KEY = "GSV_KEY";
    const char* msg;
    int SHM_BUFFER = 10000;

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
        if (monitor == 1) {
            execv(path, args);
        }
    } else {
        int fp = open("/dev/shm/GSV_KEY", "r");
        size_t pagesize = getpagesize();
        int read = fread(&fp, 1, pagesize, stdout);
        printf("Read %d pages", read);
        // while (1) {
        Time_Started = clock();
        for (int i = 0; i < iter; i++) {
            msg = shm_read(SHM_BUFFER, GSV_KEY);
            GSV = atoi(msg);

            //   printf("\nSensor || GSV from shared memory: %s\n", msg);

            // printf("\nGSV converted: %d\n", GSV);
            /*if (monitor == 1) {
            }*/

            Clock_Start = clock();
            // WORKS clock_gettime(CLOCK_REALTIME, &begin);
            sprintf(buffer, "%d", generate(1, 25000000));
            // printf("\nSensor || After Random Int Generation\n");

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

            Execution_Time[i] = (double)(Clock_End - Clock_Start) / CLOCKS_PER_SEC;

            if (Execution_Time[i] > 10000) {
                fail_count++;
                printf("Execution_Time[%d]: %f\n", i, (double)Execution_Time[i]);
                Execution_Time[i] = 0;
            } else {
                printf("Execution_Time[%d]\n", i);
                // printf("Execution_Time[%d]: %Lf\n", i, Execution_Time[i]);
                Execution_Sum += Execution_Time[i];
            }

            /* Works */
            /*
            //clock_gettime(CLOCK_REALTIME, &end);

            seconds = end.tv_sec - begin.tv_sec;
            nanoseconds = end.tv_nsec - begin.tv_nsec;

            elapsed = seconds + nanoseconds * 1e-9;
            if (elapsed > 10000) {
                fail_count++;
               // printf("fail count: %d\n", fail_count);
               // printf("Failed elapsed time: %f\n", elapsed);
            } else {
               // printf("\nelapsed time: %f\n", elapsed);
            }
            // sleep(5);
            Execution_Time[i] = elapsed;
            if (Execution_Time[i] > 10000) {
               // printf("Failed Execution_Time[%d]: %Lf\n", i, Execution_Time[i]);
                //sleep(1);
                Execution_Time[i] = 0;
               // printf("Forced Execution_Time[%d]: %Lf\n", i, Execution_Time[i]);
                sleep(1);
            } else {
                printf("Execution_Time[%d]\n", i);
                //printf("Execution_Time[%d]: %Lf\n", i, Execution_Time[i]);
            }
            // sleep(5);
            Execution_Sum += Execution_Time[i];
            if (Execution_Sum > 10000) {
                fail_count++;
               // printf("fail count: %d\n", fail_count);
               // printf("Failed Execution_Sum: %Lf\n", Execution_Sum);
                //sleep(5);
            } else {
                //printf("Execution_Sum: %Lf\n", Execution_Sum);
            }*/

            // usleep(10000);
        }
        Time_Ended = clock();

        long timestamp = (long)(Time_Ended - Time_Started);
        long milliseconds = (long)(timestamp / 1000) % 1000;
        long seconds = (((long)(timestamp / 1000) - milliseconds) / 1000) % 60;
        long minutes = (((((long)(timestamp / 1000) - milliseconds) / 1000) - seconds) / 60) % 60;
        long hours = ((((((long)(timestamp / 1000) - milliseconds) / 1000) - seconds) / 60) - minutes) / 60;

        Execution_Average = Execution_Sum / iter;
        printf("\n\n===================================\n\n");
        printf("Execution_Sum: %Lf\n", Execution_Sum);
        printf("Execution average: %Lf ms\n", Execution_Average);
        printf("Total time: %ld\n", (Time_Ended - Time_Started));
        printf("Total_Time_Elapsed [HH:MM:SS:MS]: %ld:%ld:%ld:%ld\n", hours, minutes, seconds, milliseconds);
        printf("Total failed counts: %d\n", fail_count);
        printf("\n===================================\n\n");

        //}
        // Make a log file with execution time idiot
    }
}

/*
            int shit = isnan(Execution_Time[i]);
            if (shit == 0) {
                // printf("isnan value: %d\n", isnan(Execution_Time[i]));
                Execution_Time[i] += Execution_Temp;

                if (Execution_Time[i] >= -1000000.0 || Execution_Time[i] <= 1000000.0) {
                    Execution_Sum += Execution_Time[i];
                } else if (Execution_Time[i] >= 10000000000.0000) {
                    fail_count++;
                    printf("Fail counter: %d\n", fail_count);
                    printf("Execution_Time[%d]: %Lf\n", i, Execution_Time[i]);
                    printf("Execution_Sum: %Lf\n", Execution_Time[i]);
                    //  printf("Execution_Sum exceeded 10000000\n");
                }
            } else {
                fail_count++;
                // printf("Fail counter: %d\n", fail_count);
                //  printf("isnan value: %d\n", isnan(Execution_Time[i]));
                //  printf("Execution_Time[%d]: %f\n", i, Execution_Time[i]);
            }*/
