#ifndef LIBRARIES
#define LIBRARIES
#include "Libraries.cpp"      // File with all our includes
#endif
#ifndef SOCKETS_CONT
#define SOCKETS_CONT
#include "Headers/Sockets.h"
#endif
#ifndef CONVERTER
#define CONVERTER
#include "converter.cpp"
#endif
#ifndef FUNCTIONS
#define FUNCTIONS
#include "Headers/Functions.cpp"
#endif
// #ifndef SHM_READ
// #define SHM_READ
// #include "Headers/shm_read.cpp"
// #endif

// #include<iomanip>
// #include <cstdio>
// #include<iostream>
// using namespace std;
/* -- Test Data Variable-- */
int monitor = 0;        // Disable/Enable Start of Signal Monitoring
int troubleshooting_print = 0;

// string* grid = new string[coordinates]; // Don't forget to delete [] a; when you're done!




/* Main running code */
int main(int argc, char *argv[]) {
    printf("==================\nControl Unit Process Started\n==================\n\n");

    /* ------------------------------
    -------- Conversion Area --------
    ------------- start ----------- */
    
    int hex = 1;
    int use_grid = 0;
    string* grid;

    if(argc > 1) {      // If the program is run with arguments
        cout << "\nArgument(s) accepted." << endl;

        cout << "Number of arguments combinations: " << (argc - 1)/2 << endl;

        for (int i = 1; i < argc; i++) {
            // RAM Grid Argument
            if((string) argv[i] == "-g" || (string) argv[i] == "-grid") {
                if ( (string) argv[i+1] == "no" || (string) argv[i+1] == "0" || (string) argv[i+1] == "false") {
                    cout << "=====  RAM Grid Disabled =====" << endl;
                    use_grid = 0;
                }
                else if ( (string) argv[i+1] == "yes" || (string) argv[i+1] == "1" || (string) argv[i+1] == "true") {
                    cout << "=====  RAM Grid Enabled =====" << endl;
                    use_grid = 1;
                }
            }

            // Signal Monitoring Process Argument
            if((string) argv[i] == "-m" || (string) argv[i] == "-monitor" || (string) argv[i] == "-monitoring") {
                if ( (string) argv[i+1] == "no" || (string) argv[i+1] == "0" || (string) argv[i+1] == "false") {
                    cout << "=====  Signal Monitoring Process Creation Disabled =====" << endl;
                    monitor = 0;
                }
                else if ( (string) argv[i+1] == "yes" || (string) argv[i+1] == "1" || (string) argv[i+1] == "true") {
                    cout << "=====  Signal Monitoring Process Creation Enabled =====" << endl;
                    monitor = 1;
                }
            }

            // Verbose Argument
            if((string) argv[i] == "-v" || (string) argv[i] == "-verbose") {
                if ( (string) argv[i+1] == "no" || (string) argv[i+1] == "0" || (string) argv[i+1] == "false") {
                    cout << "=====  Verbose Disabled =====" << endl;
                    troubleshooting_print = 0;
                }
                else if ( (string) argv[i+1] == "yes" || (string) argv[i+1] == "1" || (string) argv[i+1] == "true") {
                    cout << "=====  Verbose Enabled =====" << endl;
                    troubleshooting_print = 1;
                }
            }

        }
        
        if(argc == 3) {     // For enabling or disabling Hex-char output.
            hex = atoi(argv[2]);
            if(hex == 0){
                cout << "Hex-char version disabled" << endl;
            }
            else{
                cout << "Hex-char version enabled" << endl;
            }
        }
    }
    else {
        cout << "\nNo arguments inserted, running staticly." << endl;
    }
    if(use_grid == 1) {
        cout << "Using RAM Grid!" << endl;
        grid = generate_grid(x_axis, y_axis, hex);
    }

    /* ----------- end --------------
    -------- Conversion Area --------
    ------------------------------ */


    /* -- Connection Setup -- */
    uint PORT_LTE_RECEIVER = 9000;
    uint PORT_WiFi_RECEIVER = 9001;
    uint PORT_LTE_ACTUATOR = 9004;
    uint PORT_WiFi_ACTUATOR = 9005;
    const char* Actuator_IP_LTE = "10.20.0.13";     // Default: Actuator IP
    // const char* Actuator_IP_LTE = "10.20.0.16";     // Control IP
    // const char* Actuator_IP_LTE = "10.20.0.10";     // Sensor IP
    const char* Actuator_IP_WiFi = "192.168.1.143";     //Default: Actuator IP
    // const char* Actuator_IP_WiFi = "192.168.1.136";     // Control IP
    // const char* Actuator_IP_WiFi = "192.168.1.160";     // Sensor IP

    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    /* Misc */
    pthread_t T1, T2, T3, T4;

    /* Struct for message & buffer size */
    char* msg;

    /* Create sockets */
    Sockets sock;
    Sockets_Receiver(&sock, PORT_LTE_RECEIVER, PORT_WiFi_RECEIVER, LTE, WiFi);
    
    Sockets_Actuator(&sock, Actuator_IP_LTE, Actuator_IP_WiFi, PORT_LTE_ACTUATOR, PORT_WiFi_ACTUATOR, LTE, WiFi);

    if(troubleshooting_print == 1) {
        printf("Sensor LTE socket from Main(): %d\n", sock.sockLTE_RECEIVER);
        printf("Sensor WiFi socket from Main(): %d\n", sock.sockWiFi_RECEIVER);
        printf("Actuator LTE socket from Main(): %d\n", sock.act_LTE);
        printf("Actuator WiFi socket from Main(): %d\n", sock.act_WiFi);
    }

    /* Start signal monitoring process */
    pid_t signal_monitor;     // Prepare the process ID for monitoring
    signal_monitor = fork();  // Starts new process
    if (signal_monitor == 0) {
        printf("Parent Process ID: %d \n", getppid());
        printf("Monitoring Process ID is: %d \n", getpid());
        char* path = (char*) "./SignalMonitoring";                 // Path of the file for new process to run
        // char path[] = "./SignalMonitoring";                 // Path of the file for new process to run
        char* args[] = { (char*)"./SignalMonitoring&", (char*) NULL};       // Command for the function to execute, always ended on NULL argument
        if(monitor == 1) {
            execv(path, args);                                  // Tells the new process to "reset" and run a different code instead
            printf("ERROR: DIDN'T START THE MONITORING PROCESS!!\n");  // Should never get this far!
        }
    } 

    /* -- Main loop for command processing and forwarding -- */
    else {
        pthread_create(&T1, NULL, LTE_command, (void*)&sock);
        WiFi_command(sock);

        printf("\n!! -- !! -- !! -- !! -- !! -- !! -- !! -- !!\n!! -- !!  THIS SHOULD NEVER PRINT   !! -- !!\n!! -- !! -- !! -- !! -- !! -- !! -- !! -- !!");
    }
}