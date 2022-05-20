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
int data_int = 1500000;     // Static Test Variable
int monitor = 1;

// string* grid = new string[coordinates]; // Don't forget to delete [] a; when you're done!




/* Main running code */
int main(int argc, char *argv[]) {
    printf("==================\nControl Unit Process Started\n==================\n\n");

    /* ------------------------------
    -------- Conversion Area --------
    ------------- start ----------- */
/*
    string data_str = to_string(data_int);     // For "Firm" data
    int hex = 1;
    int use_grid = 1;
    string* grid;

    if(argc > 1) {      // If the program is run with arguments
        cout << "\nArgument(s) accepted." << endl;
        data_str = argv[1];

        if(data_str == "no") {
            cout << "=====  RAM Grid Disabled =====" << endl;
            use_grid = 0;
        }
        else {
            cout << "Inserted Sensor-data Number: " << argv[1] << endl;
            data_int = atoi(argv[1]);
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
    
    // print_grid(grid, coordinates);       // Printing each separate coordinate

    string cpu_command = convert_to_coordinate(data_int, hex);      // Process & Create Coordinate via CPU

    // -- Print out results -- 
    cout << "\nSensor Data is: " << data_str << endl;               // The "Sensed" Data
    if(use_grid == 1) {
        cout << "\nRAM Command is: " << grid[data_int-1] << endl;       // Coordinate from RAM version
    }
    cout << "\nCPU Command is: " << cpu_command << endl;            // Coordinate from CPU version
*/

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

    printf("Sensor LTE socket from Main(): %d\n", sock.sockLTE_RECEIVER);
    printf("Sensor WiFi socket from Main(): %d\n", sock.sockWiFi_RECEIVER);
    printf("Actuator LTE socket from Main(): %d\n", sock.act_LTE);
    printf("Actuator WiFi socket from Main(): %d\n", sock.act_WiFi);

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
        }
        printf("ERROR: DIDN'T START THE NEW PROCESS!!\n");  // Should never get this far!
    } 

    /* -- Main loop for command processing and forwarding -- */
    else {
        pthread_create(&T1, NULL, LTE_command, (void*)&sock);
        WiFi_command(sock);

        printf("\nThis shouldn't print!!!\n");
    }
}