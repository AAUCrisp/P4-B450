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
#include <iostream>
#include <sstream>
#include <string>

#include "Headers/ActuatorFunctions.cpp"
#include "Headers/SocketFunctions.h"
#include "Headers/shm_read.c"

using namespace std;

// THIS FUNCTION OPENS CHILD PROGRAM!!!
template <std::size_t N>
int execvp(const char* file, const char* const (&argv)[N]) {
    assert((N > 0) && (argv[N - 1] == nullptr));

    return execvp(file, const_cast<char* const*>(argv));
}

void* DoSomething(void* arg) {
    pthread_exit(NULL);
}

int main() {
    printf("==================\nActuator Process Started\n==================\n\n");

    /* Misc */
    pthread_t T1, T2;
    char* curr_time;
    int* Processed_Data;
    double Execution_Time = 0.0;

    /* Execution time variables */
    int count = 0;
    int fail_count = 0;
    int iter = 500000;
    long double Execution_Time[iter];
    long double Execution_Temp = 0;
    long double Execution_Sum = 0;
    long double Execution_Average = 0;
    clock_t Time_Started;
    clock_t Time_Ended;
    clock_t Clock_Start;
    clock_t Clock_End;

    /* Shared memory object variables */
    const char* COMMANDS_KEY = "COMMANDS_KEY";
    char* COMMANDS;
    char msg[1024];

    /* Create child process */
    pid_t Actuator_monitor;     // Prepare the process ID for monitoring
    Actuator_monitor = fork();  // Starts new process

    if (Actuator_monitor == 0) {
        printf("Parent process ID: %d \n", getppid());
        printf("Actuator monitoring process ID is: %d \n", getpid());
        char path[] = "./ActuatorMonitoring";
        const char* args[] = {"./ActuatorMonitoring", NULL};
        // execvp(path, args);

    } else {
        Time_Started = clock();
        while (1) {
            count++;
            pthread_create(&T1, NULL, DoSomething, NULL);
            COMMANDS = (char*)shm_read(32, COMMANDS_KEY);
            printf("COMMANDS from shared memory: %s\n", COMMANDS);

            snprintf(msg, sizeof(msg), "%s", COMMANDS);

            Clock_Start = clock();
            processData(COMMANDS);
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
            if (count == iter) {
                break;
            }
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
    }
}
