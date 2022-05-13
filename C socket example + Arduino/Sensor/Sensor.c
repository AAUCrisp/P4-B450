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

#include "Sockets_TX_RX.h"
#include "shm_write_read.h"

int main() {
    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE_RECEIVE = 6969;
    uint PORT_WiFi_RECEIVE = 6968;
    uint PORT_LTE_TRANSMIT = 9121;
    uint PORT_WiFi_TRANSMIT = 9122;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    /* Misc */
    pthread_t T1, T2, T3, T4;

    /* Struct for message & buffer size */
    char* msg;

    /* Create sockets */
    Sockets sock;
    Create_Bind_Sockets(&sock, PORT_LTE, PORT_WiFi, PORT_LTE_TRANS, PORT_WiFi_TRANS, LTE, WiFi);
    printf("sockLTE control_unit: %d\n", sock.sockLTE);
    printf("sockWiFi control_unit: %d\n", sock.sockWiFi);

    pid_t sensor_monitor;     // Prepare the process ID for monitoring
    sensor_monitor = fork();  // Starts new process
    if (sensor_monitor == 0) {
        printf("Parent process ID: %d \n", getppid());
        printf("Sensor monitoring process ID is: %d \n", getpid());
        char path[] = "./SensorMonitoring";
        char* args[] = {"./SensorMonitoring&", NULL};
        execv(path, args);
    } else {
        while (1) {
            pthread_create(&T1, NULL, /*Function*/, (void*)&sock);
            pthread_create(&T2, NULL, /*Function*/, (void*)&sock);
        }
    }
}