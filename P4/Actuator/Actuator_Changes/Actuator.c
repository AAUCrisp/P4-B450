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

#include "Headers/ActuatorFunctions.h"
#include "Headers/SocketFunctions.h"
#include "Headers/shm_write_read.h"

using namespace std;

int main() {
    printf("==================\nActuator Process Started\n==================\n\n");

    /* Misc */
    pthread_t T1, T2;
    char* curr_time;
    int* process_data;

    /* Shared memory object variables */
    const char* COMMANDS_KEY = "COMMANDS_KEY";
    const char* COMMANDS;
    char msg[1024];

    /* Create child process */
    pid_t sensor_monitor;     // Prepare the process ID for monitoring
    sensor_monitor = fork();  // Starts new process

    if (sensor_monitor == 0) {
        printf("Parent process ID: %d \n", getppid());
        printf("Actuator monitoring process ID is: %d \n", getpid());
        char path[] = "./ActuatorMonitoring";
        char* args[] = {"./ActuatorMonitoring", NULL};
        execv(path, args);

    } else {
        while (1) {
            COMMANDS = shm_read(32, COMMANDS_KEY);
            printf("GSV from shared memory: %s\n", COMMANDS);

            snprintf(msg, sizeof(msg), "%s", COMMANDS);

            process_data = processData(COMMANDS);
            logData(process_data);
        }
    }
}
