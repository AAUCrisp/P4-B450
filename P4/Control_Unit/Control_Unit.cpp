#ifndef LIBRARIES
#define LIBRARIES
#include "Libraries.cpp"      // File with all our includes
#endif
#ifndef SOCKETS_CONT
#define SOCKETS_CONT
#include "Headers/Sockets.h"
#endif
#include "converter.cpp"

/* -- Test Data Variable-- */
int data_int = 1500000;     // Static Test Variable
// string* grid = new string[coordinates]; // Don't forget to delete [] a; when you're done!

/* Main running code */
int main(int argc, char *argv[]) {

    /* ------------------------------
    -------- Conversion Area --------
    ------------- start ----------- */

    string data_str = to_string(data_int);     // For "Firm" data
    int hex = 1;

    if(argc > 1) {      // If the program is run with arguments
        cout << "\nArgument(s) accepted." << endl;
        cout << "Inserted Sensor-data Number: " << argv[1] << endl;
        data_str = argv[1];
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

    else {
        cout << "\nNo arguments inserted, running staticly." << endl;
    }
    
    string* grid = generate_grid(x_axis, y_axis, hex);
    
    // print_grid(grid, coordinates);       // Printing each separate coordinate

    string cpu_command = convert_to_coordinate(data_int, hex);      // Process & Create Coordinate via CPU

    // -- Print out results -- 
    cout << "\nSensor Data is: " << data_str << endl;               // The "Sensed" Data
    cout << "\nRAM Command is: " << grid[data_int-1] << endl;       // Coordinate from RAM version
    cout << "\nCPU Command is: " << cpu_command << endl;            // Coordinate from CPU version


    /* ----------- end --------------
    -------- Conversion Area --------
    ------------------------------ */


    /* -- Connection Setup -- */
    uint PORT_LTE_RECEIVER = 9000;
    uint PORT_WiFi_RECEIVER = 9001;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    /* Misc */
    pthread_t T1, T2, T3, T4;

    /* Struct for message & buffer size */
    char* msg;

    /* Create sockets */
    Sockets sock;
    Sockets_Receiver(&sock, PORT_LTE_RECEIVER, PORT_WiFi_RECEIVER, LTE, WiFi);
    printf("sockLTE_RECEIVER (OUTSIDE): %d\n", sock.sockLTE_RECEIVER);
    printf("sockWiFi_RECEIVER (OUTSIDE): %d\n", sock.sockWiFi_RECEIVER);

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
    close(sock.sockLTE_TRANSMITTER);
    close(sock.sockWiFi_TRANSMITTER);

    close(sock.sockLTE_RECEIVER);
    close(sock.sockWiFi_RECEIVER);
    exit(0);
}