#ifndef LIBRARIES
#define LIBRARIES
#include "Libraries.c"  // File with all our includes
#endif
#ifndef FUNCTIONS
#define FUNCTIONS
#include "Headers/Functions.c"
#endif

#include "Headers/SocketFunctions.h"
#include "Headers/shm_read_write.h"

#define BUFFER 64
#define BILLION 1000000000.0

int main(int argc, char* argv[]) {
    // int both_tech = 0;
    // int monitor = 1;
    // int iter = 10000;

    printf("\n=======================\nSensor Program Started\n=======================\n\n");

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
    //const char* IP_WiFi = "192.168.1.136";  // Default: IP of Control Unit
    // const char* IP_WiFi = "192.168.1.143";  // IP of Actuator
    // const char* IP_WiFi = "192.168.1.160";  // IP of Sensor
    const char* IP_WiFi = "10.42.0.1"; // Default: IP of Control Unit (AP)


    // const char* LTE = "lo";             // Test loopback
    // const char* WiFi = "lo";            // Test loopback
    // const char* IP_LTE = "127.0.0.1";   // Test loopback
    // const char* IP_WiFi = "127.0.0.1";  // Test loopback

    /* misc */
    pthread_t T1;

    /* Execution time variables */
    int count = 0;
    int fail_count = 0;
    long double Execution_Sum = 0;
    long double Execution_Average = 0;

    struct timespec begin, end, begin_program, end_program;
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
    int* SHM_BUFFER[BUFFER];
    char buffer[BUFFER];

    char* gsv;
    char* B = "0";
    char* W = "1";
    char* L = "2";

    void* dummy() {
        pthread_exit(NULL);
    }

    /* Check status of global variables */
    printf("\n===================================\n");
    printf("\nboth_tech: %d\n", both_tech);
    printf("monitor: %d\n", monitor);
    printf("iter: %d\n", iter);
    printf("delay: %d\n", delay);
    printf("GSV_default: %s\n", GSV_default);
    printf("\n===================================\n");

    /* Create child process */
    // printf("both_tech: %d\n", both_tech);
    pid_t sensor_monitor;  // Prepare the process ID for monitoring
    if (monitor == 1) {
        sensor_monitor = fork();  // Starts new process
    }

    /* Checks if child process is running */
    if (sensor_monitor == 0) {
        printf("Parent process ID: %d \n", getppid());
        printf("Sensor monitoring process ID is: %d \n", getpid());
        char path[] = "./SensorMonitoring";
        char* args[] = {"./SensorMonitoring", NULL};
        // if (monitor == 1) {
        execv(path, args);
        printf("  ERROR: DIDN'T START THE MONITORING PROCESS!!\n");  // Should never get this far!
        // }
        printf("Why no work?2\n");
    } else {
        // printf("Why no work?3\n");
        // pthread_create(&T1, NULL, dummy, NULL);
        /* Initialize SHM object reading */
        gsv = shm_read(BUFFER, GSV_KEY);

        /* Start timing all code */
        clock_gettime(CLOCK_REALTIME, &begin_program);

        while (1) {
            // printf("\nSensor || GSV from shared memory: %s\n", (char*)gsv);

            // printf("\nGSV converted: %d\n", GSV);
            /*if (monitor == 1) {
            }*/

            /* Start timing code execution of code */
            clock_gettime(CLOCK_REALTIME, &begin);

            sprintf(buffer, "%d: %d", count, generate(1, 25000000));
            printf("Packet ID + random int: %s\n", buffer);

            /*if (both_tech == 1) {
                printf("\nSensor || Troubleshooting for Both Technologies\n");
                GSV = 0;  // Troubleshooting for both
            }*/

            // printf("Sensor || Before Transmitting\n");
            if (strcmp(gsv, B) == 0 || strcmp(gsv, W) == 0) {
                transmitLTE(&sock, (char*)buffer);
            }

            if (strcmp(gsv, B) == 0 || strcmp(gsv, L) == 0) {
                transmitWiFi(&sock, (char*)buffer);
            }

            /* Stop timing code execution of code */
            clock_gettime(CLOCK_REALTIME, &end);

            seconds = end.tv_sec - begin.tv_sec;
            nanoseconds = end.tv_nsec - begin.tv_nsec;

            /* Calculation of elapsed time sum */
            elapsed = seconds + nanoseconds * 1e-9;
            if (elapsed > 10000) {
                fail_count++;
                elapsed = 0;
            }
            Execution_Sum += elapsed;
            count++;

            if (count == iter) {
                break;
            }
            usleep(delay);
        }
        /* Stop timing all code */
        clock_gettime(CLOCK_REALTIME, &end_program);

        /* Calculation of total time execution */
        double time_spent = (end_program.tv_sec - begin_program.tv_sec) +
                            (end_program.tv_nsec - begin_program.tv_nsec) / BILLION;

        /* Conversion of time spent to HH:MM:SS.MS */
        long hours = (long)time_spent / 3600;
        long minutes = ((long)time_spent / 60) % 60;
        long seconds = (long)time_spent % 60;
        long milliseconds = (long)(time_spent * 1000) % 1000;

        /* Calculation of execution average */
        Execution_Average = Execution_Sum / iter;

        printf("\n\n===================================\n\n");
        printf("Execution Sum:     %Lf sec\n", Execution_Sum);
        printf("Execution average: %Lf sec\n\n", Execution_Average);
        printf("Total time: %f sec\n", time_spent);
        printf("________________________\n");
        printf("Total Time:  \n            Hours: %ld  \n          Minutes: %ld  \n          Seconds: %ld \n     Milliseconds: %ld\n", hours, minutes, seconds, milliseconds);
        printf("________________________\n\n");
        printf("Total failed counts: %d\n", fail_count);
        printf("\n===================================\n\n");
    }
}
