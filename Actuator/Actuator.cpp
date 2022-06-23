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
//#include "Headers/SocketFunctions.h"
#include <iostream>
#include "Headers/shm_read_write.h"
#include "Headers/ActuatorFunctions.h"

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
    int iter = 0;

    /* Execution time variables */
    int count = 0;
    int fail_count = 0;
    long double Execution_Sum = 0;
    long double Execution_Average = 0;

    struct timespec begin, end, begin_program, end_program;
    unsigned long seconds = 0;
    unsigned long nanoseconds = 0;
    double elapsed = 0;

    /* Shared memory object variables */
    const char* COMMANDS_KEY = "COMMANDS_KEY";
    char* COMMANDS;
    char msg[1024];

    /* Create child process */
    pid_t Actuator_monitor;     // Prepare the process ID for monitoring
    //Actuator_monitor = fork();  // Starts new process

    /* Checks if child process is running */
    if (Actuator_monitor == 0) {
        printf("Parent process ID: %d \n", getppid());
        printf("Actuator monitoring process ID is: %d \n", getpid());
        char path[] = "./ActuatorMonitoring";
        const char* args[] = {"./ActuatorMonitoring", NULL};
        //execvp(path, (char* const*)args); // test

    } else {
        /* Initialize SHM object reading */
        COMMANDS = (char*)shm_read(32, COMMANDS_KEY);

        /* Start timing all code */
        clock_gettime(CLOCK_REALTIME, &begin_program);

        while (1) {
            
            printf("COMMANDS from shared memory: %s\n", COMMANDS);

            snprintf(msg, sizeof(msg), "%s", COMMANDS);  // Is "msg" even used for anything???

            /* Start timing code execution of code */
            clock_gettime(CLOCK_REALTIME, &begin);

            processData(COMMANDS);

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
            printf("count: %d\n", count);
            if (count == iter) {
                break;
            }
        }
    }
    /* Stop timing all code */
    clock_gettime(CLOCK_REALTIME, &end_program);

    /* Calculation of total time execution */
    double time_spent = (end_program.tv_sec - begin_program.tv_sec) +
                        (end_program.tv_nsec - begin_program.tv_nsec) / BILLION;

    /* Conversion of time spent to HH:MM:SS.MS */
    long hours = (long)time_spent / 3600;
    long minutes = ((long)time_spent / 60) % 60;
    long seconds2 = (long)time_spent % 60;
    long milliseconds = (long)(time_spent * 1000) % 1000;

    /* Calculation of execution average */
    Execution_Average = Execution_Sum / iter;

    printf("\n\n===================================\n\n");
    printf("Execution Sum:     %Lf sec\n", Execution_Sum);
    printf("Execution average: %Lf sec\n\n", Execution_Average);
    printf("Total time: %f sec\n", time_spent);
    printf("________________________\n");
    printf("Total Time:  \n            Hours: %ld  \n          Minutes: %ld  \n          Seconds: %ld \n     Milliseconds: %ld\n", hours, minutes, seconds2, milliseconds);
    printf("________________________\n\n");
    printf("Total failed counts: %d\n", fail_count);
    printf("\n===================================\n\n");
}
