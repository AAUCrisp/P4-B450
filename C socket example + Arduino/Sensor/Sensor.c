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
    uint PORT_LTE_TRANSMITTER = 9000;
    uint PORT_WiFi_TRANSMITTER = 9001;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";
    const char* IP_LTE = "10.20.0.16";      // IP of server
    const char* IP_WiFi = "192.168.1.136";  // IP of server

    /* Misc */
    pthread_t T1, T2;

    /* Create sockets */
    Sockets sock;
    Sockets_Transmitter(&sock, IP_LTE, IP_WiFi, PORT_LTE_TRANSMITTER, PORT_WiFi_TRANSMITTER, LTE, WiFi);
    printf("sockLTE_TRANSMITTER (OUTSIDE): %d\n", sock.sockLTE_TRANSMITTER);
    printf("sockWiFi_TRANSMITTER (OUTSIDE): %d\n", sock.sockWiFi_TRANSMITTER);

    /* Shared memory object variables */
    const char* GSV_KEY = "GSV_KEY";
    const char* msg;

    const char* RAND_KEY = "RAND_KEY";
    int rand_int;

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
        printf("Does it reach else?\n");
        while (1) {
            printf("Does it reach while loop?\n");
            sleep(2);
            msg = shm_read(32, GSV_KEY);
            GSV = atoi(msg);
            printf("GSV from shared memory: %s\n", msg);
            printf("GSV converted: %d\n", GSV);
            rand_int = generate(0, 2500);

            if (GSV == B || GSV == L) {
                /*printf("Shared memory GSV thing works! %s\n", msg);
                printf("SHIT DO WORK! %d\n", GSV);*/
                Timestamp();
                // char* itoa(int_1, rand_int, 10);
                printf("Random int to char: %d\n", rand_int);
                shm_write(rand_int, 32, RAND_KEY);
                pthread_create(&T1, NULL, transmitLTE, (void*)&sock);
            }

            if (GSV == B || GSV == W) {
                Timestamp();
                // int rand_int = generate(0, 2500);
                // char* itoa(int_2, rand_int, 10);
                shm_write(rand_int, 32, RAND_KEY);
                printf("Random int to char: %d\n", rand_int);
                pthread_create(&T2, NULL, transmitWiFi, (void*)&sock);
            }
        }
    }
}