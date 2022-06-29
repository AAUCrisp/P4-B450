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

#include <cassert>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

//#include "Headers/ActuatorFunctions.cpp"
#include <iostream>

#include "Headers/ActuatorFunctions.h"
#include "Headers/SocketFunctions.h"
#include "Headers/shm_read_write.h"

using namespace std;

#define SHM_BUFFER 100
#define BILLION 1000000000.0

// THIS FUNCTION OPENS CHILD PROGRAM!!!
/*
template <std::size_t N>
int execvp(const char* file, const char* const (&argv)[N]) {
    assert((N > 0) && (argv[N - 1] == nullptr));

    return execvp(file, const_cast<char* const*>(argv));
}*/

int main() {
    printf("==================\nActuator Process Started\n==================\n\n");

    /* Misc */
    pthread_t T1, T2;
    char* curr_time;
    int* Processed_Data;
    int iter = 1000000;

   

    /* Shared memory object variables */
    const char* COMMANDS_KEY = "COMMANDS_KEY";
    char* COMMANDS;
    char msg[1024];

    const char* stop_key = "STOP_KEY";
    char* stopshit;

    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE_RECEIVER = 9004;
    uint PORT_WiFi_RECEIVER = 9005;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    // const char* LTE = "enp0s3";         // Test loopback
    // const char* WiFi = "enp0s3";        // Test loopback
    // const char* LTE = "lo";             // Test loopback
    // const char* WiFi = "lo";            // Test loopback

    /* Create sockets */
    Sockets sock;
    Sockets_Receiver(&sock, PORT_LTE_RECEIVER, PORT_WiFi_RECEIVER, LTE, WiFi);
    printf("sockLTE_RECEIVER (OUTSIDE): %d\n", sock.sockLTE_RECEIVER);
    printf("sockWiFi_RECEIVER (OUTSIDE): %d\n\n", sock.sockWiFi_RECEIVER);

    /* Execution time variables */
    struct timespec begin_program, end_program;
    unsigned long seconds = 0;
    unsigned long nanoseconds = 0;
    long double Execution_Average = 0;

    sock.Execution_Sum_WiFi = 0;
    sock.Execution_Sum_LTE = 0;
    sock.packet_count_WiFi = 0;
    sock.packet_count_LTE = 0;

    /* Create child process */
    pid_t Actuator_monitor;  // Prepare the process ID for monitoring
    // Actuator_monitor = fork();  // Starts new process

    /* Checks if child process is running */
    if (Actuator_monitor == 0) {
        printf("Parent process ID: %d \n", getppid());
        printf("Actuator monitoring process ID is: %d \n", getpid());
        char path[] = "./ActuatorMonitoring";
        const char* args[] = {"./ActuatorMonitoring", NULL};
        // execvp(path, (char* const*)args);

    } else {
        sock.packet_count_LTE = 0;
        sock.packet_count_WiFi = 0;
        cout << "\n\npacket count LTE: " << sock.packet_count_LTE << endl;
        cout << "packet count WiFi: " << sock.packet_count_WiFi << endl;

        /* Start timing all code */
        clock_gettime(CLOCK_REALTIME, &begin_program);

        pthread_create(&T1, NULL, receiveLTE, (void*)&sock);
        receiveWiFi(&sock);

        cout << "\n\nNEW packet count LTE: " << sock.packet_count_LTE << endl;
        cout << "NEW packet count WiFi: " << sock.packet_count_WiFi << endl;
    }

    /* Stop timing all code */
    clock_gettime(CLOCK_REALTIME, &end_program);

    /* Calculation of total time execution */
    double time_spent = ((end_program.tv_sec - begin_program.tv_sec) +
                         (end_program.tv_nsec - begin_program.tv_nsec) / BILLION) -
                        5;

    /* Conversion of time spent to HH:MM:SS.MS */
    long hours = (long)time_spent / 3600;
    long minutes = ((long)time_spent / 60) % 60;
    long seconds2 = (long)time_spent % 60;
    long milliseconds = (long)(time_spent * 1000) % 1000;

    /* Calculation of execution average */
    Execution_Average = (sock.Execution_Sum_WiFi + sock.Execution_Sum_LTE) / (sock.packet_count_WiFi + sock.packet_count_LTE);

    printf("\n\n===================================\n\n");
    printf("Execution time sums:\n");
    printf("    Total Execution Sum:     %Lf sec\n", (sock.Execution_Sum_LTE + sock.Execution_Sum_WiFi));
    printf("     WiFi Execution Sum:     %Lf sec\n", sock.Execution_Sum_WiFi);
    printf("      LTE Execution Sum:     %Lf sec\n\n", sock.Execution_Sum_LTE);
    printf("Execution time average: \n");
    printf("    Total Execution average: %Lf sec\n\n", Execution_Average);
    printf("Total program time: \n");
    printf("    Total time: %f sec\n", time_spent);
    printf("________________________\n\n");
    printf("Total Time:  \n            Hours: %ld  \n          Minutes: %ld  \n          Seconds: %ld \n     Milliseconds: %ld\n", hours, minutes, seconds2, milliseconds);
    printf("________________________\n\n");
    printf("Total failed counts via WiFi:       %d\n", sock.fail_count_WiFi);
    printf("Total failed counts via LTE:        %d\n", sock.fail_count_LTE);
    printf("Total packets received via WiFi:    %d\n", sock.packet_count_WiFi);
    printf("Total packets received via LTE:     %d\n", sock.packet_count_LTE);
    printf("Total packets received via testcountLTE:     %d\n", sock.testcountLTE);
    printf("\n===================================\n\n");
}
