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

#include "Headers/Sockets.h"
#include "Headers/shm_write_read.h"

/* Main running code */
int main() {
    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE = 9000;
    uint PORT_WiFi = 9001;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";
    /* Misc */
    pthread_t T1, T2;

    /* Struct for message & buffer size */
    char* msg;

    /* Create sockets */
    Sockets sock;
    Sockets_Receiver(&sock, PORT_LTE, PORT_WiFi, LTE, WiFi);
    printf("sockLTE_TRANSMITTER: %d\n", sock.sockLTE_TRANSMITTER);
    printf("sockWiFi_TRANSMITTER: %d\n", sock.sockWiFi_TRANSMITTER);

    /* Start signal monitoring process */
    pid_t signal_monitor;     // Prepare the process ID for monitoring
    signal_monitor = fork();  // Starts new process
    if (signal_monitor == 0) {
        printf("Parent Process ID: %d \n", getppid());
        printf("Monitoring Process ID is: %d \n", getpid());
        char path[] = "./SignalMonitoring";            // Path of the file for new process to run
        char* args[] = {"./SignalMonitoring&", NULL};  // Command for the function to execute, always ended on NULL argument
        execv("./SignalMonitoring", args);                                  // Tells the new process to "reset" and run a different code instead
        printf("ERROR: DIDN'T START THE NEW PROCESS!!\n");  // Should never get this far!
    } else {
        while (1) {
            // printf("==================\nMain Control Unit Process Started\n==================\n\n");
            Timestamp();
            printf("Test, does this print?");
            pthread_create(&T1, NULL, receiveLTE, (void*)&sock);
            printf("Test, does this print?2");
            pthread_join(T1, (void**)&msg);
            printf("Test, does this print?3");
            Timestamp();
            pthread_create(&T2, NULL, receiveWiFi, (void*)&sock);
            pthread_join(T2, (void**)&msg);
            printf("THIS IS RECEIVED MESSAGE: %s\n", msg);
            sleep(5);
        }
    }

    close(sock.sockLTE_TRANSMITTER);
    close(sock.sockWiFi_TRANSMITTER);

    close(sock.sockLTE_RECEIVER);
    close(sock.sockWiFi_RECEIVER);
    exit(0);
}