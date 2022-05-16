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

int generate(int Min, int Max) {
    int number = (rand() % ((Max + 1) - Min)) + Min;
    return number;
}

int main() {
    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE = 9000;
    uint PORT_WiFi = 9001;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    /* Misc */
    pthread_t T1, T2;

    /* Create sockets */
    Sockets sock;
    Sockets_Receiver(&sock, PORT_LTE, PORT_WiFi, LTE, WiFi);
    printf("sockLTE_TRANSMITTER: %d\n", sock.sockLTE_TRANSMITTER);
    printf("sockWiFi_TRANSMITTER: %d\n", sock.sockWiFi_TRANSMITTER);

    /* Shared memory object variables */
    const char* GSV_KEY = "GSV_KEY";
    const char* msg;

    const char* RAND_KEY = "RAND_KEY";
    const char* rand_int;

    int GSV;
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
        execv(path, args);

    } else {
        msg = shm_read(10, GSV_KEY);
        GSV = atoi(msg);
        printf("GSV from shared memory: %s\n", msg);
        printf("GSV converted: %d\n", GSV);

        while (1) {
            sleep(2);
            if (GSV == B || GSV == L) {
                /*printf("Shared memory GSV thing works! %s\n", msg);
                printf("SHIT DO WORK! %d\n", GSV);*/
                Timestamp();
                sprintf(rand_int, "%s", generate(0, 2500));
                printf("Random int to char: %s\n", rand_int);
                shm_write(rand_int, 10, RAND_KEY);
                pthread_create(&T1, NULL, transmitLTE, (void*)&sock);
            }

            if (GSV == B || GSV == W) {
                Timestamp();
                sprintf(rand_int, "%s", generate(0, 2500));
                shm_write(rand_int, 10, RAND_KEY);
                printf("Random int to char: %s\n", rand_int);
                pthread_create(&T2, NULL, transmitWiFi, (void*)&sock);
            }
        }
    }
}