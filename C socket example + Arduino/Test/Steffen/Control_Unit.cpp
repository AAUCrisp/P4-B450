#ifndef LIBRARIES 
#define LIBRARIES
#include "Libraries.cpp"      // File with all our includes
#endif
#ifndef SOCKETS_CONT
#define SOCKETS_CONT
#include "Headers/Sockets.h"
#endif


/* Main running code */
int main() {
    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE = 6969;
    uint PORT_WiFi = 6968;
    uint PORT_LTE_TRANS = 1;
    uint PORT_WiFi_TRANS = 1;
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

    /* Start signal monitoring process */
    pid_t signal_monitor;     // Prepare the process ID for monitoring
    signal_monitor = fork();  // Starts new process
    if (signal_monitor == 0) {
        printf("Parent Process ID: %d \n", getppid());
        printf("Monitoring Process ID is: %d \n", getpid());
        char* path = (char*) "./SignalMonitoring";                 // Path of the file for new process to run
        // char path[] = "./SignalMonitoring";                 // Path of the file for new process to run
        char* args[] = { (char*)"./SignalMonitoring&", (char*) NULL};       // Command for the function to execute, always ended on NULL argument
        execv(path, args);                                  // Tells the new process to "reset" and run a different code instead
        printf("ERROR: DIDN'T START THE NEW PROCESS!!\n");  // Should never get this far!
    } else {
        while (1) {
            // printf("==================\nMain Control Unit Process Started\n==================\n\n");
            Timestamp();
            pthread_create(&T1, NULL, receiveLTE, (void*)&sock);
            pthread_join(T1, (void**)&msg);
            Timestamp();
            pthread_create(&T2, NULL, receiveWiFi, (void*)&sock);
            pthread_join(T2, (void**)&msg);
            printf("%s\n", msg);
            sleep(5);
        }
    }

    close(sock.sockLTE && sock.sockWiFi);
    exit(0);
}