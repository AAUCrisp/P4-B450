#ifndef LIBRARIES
#define LIBRARIES
#include "Libraries.cpp"  // File with all our includes
#endif
#ifndef SOCKETS_CONT
#define SOCKETS_CONT
#include "Headers/Sockets.h"
// #include "Headers/Sockets.cpp"
#endif
#ifndef CONVERTER
#define CONVERTER
#include "converter.cpp"
#endif
#ifndef FUNCTIONS
#define FUNCTIONS
#include "Headers/Functions.cpp"
#endif

//#include "Headers/shm_read_write.h"

/* Main running code */
int main(int argc, char* argv[]) {
    // string* grid;

    // If Arguments is inserted
    if (argc > 1) {  // If the program is run with arguments
        Argument_Setup(argc, argv);
    } else {  // Running without arguments
        cout << "  ===================================" << endl;
        cout << "     Control Unit Process Started" << endl;
        cout << "  ===================================" << endl
             << endl
             << endl
             << endl;
        cout << "\n  No arguments inserted." << endl;
    }

    /* -- Connection Setup -- */
    uint PORT_LTE_RECEIVER = 9000;
    uint PORT_WiFi_RECEIVER = 9001;
    uint PORT_LTE_ACTUATOR = 9004;
    uint PORT_WiFi_ACTUATOR = 9005;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";
    const char* Actuator_IP_LTE = "10.20.0.13";    // Default: Actuator IP
                                                   //  const char* Actuator_IP_LTE = "10.20.0.16";     // Control IP
                                                   //  const char* Actuator_IP_LTE = "10.20.0.10";     // Sensor IP
    const char* Actuator_IP_WiFi = "10.42.0.118";  // Default: Actuator IP (AP)
    // const char* Actuator_IP_WiFi = "192.168.1.143";     //Default: Actuator IP
    //  const char* Actuator_IP_WiFi = "192.168.1.136";     // Control IP
    //  const char* Actuator_IP_WiFi = "192.168.1.160";     // Sensor IP

    // const char* Actuator_IP_LTE = "127.0.0.1";   // test loopback
    // const char* Actuator_IP_WiFi = "127.0.0.1";  // test loopback
    // const char* LTE = "lo";                      // test loopback
    // const char* WiFi = "lo";                     // test loopback

    /* Misc */
    pthread_t T1;

    /* Struct for message & buffer size */
    char* msg;

    /* Create sockets */
    Sockets sock;
    Sockets_Receiver(&sock, PORT_LTE_RECEIVER, PORT_WiFi_RECEIVER, LTE, WiFi);
    Sockets_Actuator(&sock, Actuator_IP_LTE, Actuator_IP_WiFi, PORT_LTE_ACTUATOR, PORT_WiFi_ACTUATOR, LTE, WiFi);

    if (troubleshooting_print == 1) {
        printf("  Sensor LTE socket from Main(): %d\n", sock.sockLTE_RECEIVER);
        printf("  Sensor WiFi socket from Main(): %d\n", sock.sockWiFi_RECEIVER);
        printf("  Actuator LTE socket from Main(): %d\n", sock.act_LTE);
        printf("  Actuator WiFi socket from Main(): %d\n", sock.act_WiFi);
    }

    /* Start signal monitoring process */
    pid_t signal_monitor;  // Prepare the process ID for monitoring
    if (monitor == 1) {
        signal_monitor = fork();  // Starts new process
        printf("signal_monitor value: %d\n", signal_monitor);
    }
    if (signal_monitor == 0) {
        if (troubleshooting_print == 1) {
            printf("  Parent Process ID: %d \n", getppid());
            printf("  Monitoring Process ID is: %d \n", getpid());
        }

        char* path = (char*)"./SignalMonitoring";                                                                                               // Path of the file for new process to run
        char* args[] = {(char*)"./SignalMonitoring&", GSV_arg_used, GSV_print, GSV_sleep, GSV_sleep_arg, GSV_tech, GSV_tech_arg, (char*)NULL};  // Command for the function to execute, always ended on NULL argument
        // execv(path, args);                                  // Tells the new process to "reset" and run a different code instead
        printf("  ERROR: DIDN'T START THE MONITORING PROCESS!!\n");  // Should never get this far!
    }

    /* -- Main loop for command processing and forwarding -- */
    else {
        pthread_create(&T1, NULL, LTE_command, (void*)&sock);
        WiFi_command(sock);

        printf("\n!! -- !! -- !! -- !! -- !! -- !! -- !! -- !!\n!! -- !!  THIS SHOULD NEVER PRINT   !! -- !!\n!! -- !! -- !! -- !! -- !! -- !! -- !! -- !!");
    }
}