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
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "Sockets_TX_RX.h"

/* Main running code */
int main() {
    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE = 9123;
    uint PORT_WiFi = 9124;
    uint PORT_LTE_TRANS = 9121;
    uint PORT_WiFi_TRANS = 9122;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    /* Misc */
    pthread_t T1, T2;

    /* Struct for message & buffer size */
    char* msg;

    /* Create sockets */
    Sockets sock;
    Create_Bind_Sockets(&sock, PORT_LTE, PORT_WiFi, PORT_LTE_TRANS, PORT_WiFi_TRANS, LTE, WiFi);
    printf("sockLTE control_unit: %d\n", sock.sockLTE);
    printf("sockWiFi control_unit: %d\n", sock.sockWiFi);

    /* Start signal monitoring process */
    pid_t signal_monitor;     // Prepare the process ID for monitoring
    signal_monitor = fork();  // Starts new process
    if (signal_monitor == 0) {
        printf("Parent Process ID: %d \n", getppid());
        printf("Monitoring Process ID is: %d \n", getpid());
        char path[] = "./SignalMonitoring";                 // Path of the file for new process to run
        char* args[] = {"./SignalMonitoring", NULL};        // Command for the function to execute, always ended on NULL argument
        execv(path, args);                                  // Tells the new process to "reset" and run a different code instead
        printf("ERROR: DIDN'T START THE NEW PROCESS!!\n");  // Should never get this far!
    } else {
        while (1) {
            //printf("Hello i am in while\n");
            Timestamp();
            pthread_create(&T1, NULL, receiveLTE, (void*)&sock);
            Timestamp();
            pthread_create(&T2, NULL, receiveWiFi, (void*)&sock);
            // pthread_join(T1, (void**)&msg);
            // pthread_join(T2, (void**)&msg);

            // printf("%s\n", msg);
            // sleep(1);
        }
    }

    close(sock.sockLTE && sock.sockWiFi);
    exit(0);
}