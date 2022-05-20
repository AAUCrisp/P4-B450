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

void* DoSomething(void * arg) {
    pthread_exit(NULL);
}

int main() {
    printf("==================\nActuator Process Started\n==================\n\n");

    /* Misc */
    pthread_t T1, T2;
    char* curr_time;
    int *Processed_Data;

    /* Shared memory object variables */
    const char* COMMANDS_KEY = "COMMANDS_KEY";
    char* COMMANDS;
    char msg[1024];

    /* Create child process */
    pid_t Actuator_monitor;  // Prepare the process ID for monitoring
    // Actuator_monitor = fork();  // Starts new process

    if (Actuator_monitor == 0) {
        printf("Parent process ID: %d \n", getppid());
        printf("Actuator monitoring process ID is: %d \n", getpid());
        char path[] = "./ActuatorMonitoring";
        const char* args[] = {"./ActuatorMonitoring", NULL};
        execvp(path, args);

    } else {
        while (1) {
            pthread_create(&T1, NULL, DoSomething, NULL);
            COMMANDS = (char*)shm_read(32, COMMANDS_KEY);
            printf("GSV from shared memory: %s\n", COMMANDS);

            snprintf(msg, sizeof(msg), "%s", COMMANDS);

            Processed_Data = processData(COMMANDS);
            //printf("Processed_Data: %d\n%d", Processed_Data)
            logData(Processed_Data);
            sleep(2);
        }
    }
}
