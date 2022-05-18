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

int main() {
    printf("==================\nSensor Process Started\n==================\n\n");

    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE_TRANSMITTER = 9000;
    uint PORT_WiFi_TRANSMITTER = 9001;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";
    const char* IP_LTE = "10.20.0.16";      // IP of server
    const char* IP_WiFi = "192.168.1.136";  // IP of server

    /* Misc */
    pthread_t T1, T2;
    char* curr_time;

    /* Create sockets */
    Sockets sock;
    Sockets_Transmitter(&sock, IP_LTE, IP_WiFi, PORT_LTE_TRANSMITTER, PORT_WiFi_TRANSMITTER, LTE, WiFi);
    printf("sockLTE_TRANSMITTER (OUTSIDE): %d\n", sock.sockLTE_TRANSMITTER);
    printf("sockWiFi_TRANSMITTER (OUTSIDE): %d\n", sock.sockWiFi_TRANSMITTER);

    /* Shared memory object variables */
    const char* GSV_KEY = "GSV_KEY";
    const char* RAND_INT_KEY = "RAND_INT_KEY";
    const char* RAND_INT;
    const char* msg;

    char buffer[BUFFER];

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
        //execv(path, args);

    } else {
        pthread_create(&T1, NULL, transmitLTE, (void*)&sock);
        pthread_create(&T2, NULL, transmitWiFi, (void*)&sock);
        while (1) {
            sprintf(buffer, "%d", generate(0, 2500));
            shm_write(buffer, 32, RAND_INT_KEY);
        }
        sleep(3);
    }
}
