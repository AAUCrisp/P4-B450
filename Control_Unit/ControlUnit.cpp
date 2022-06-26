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

#define BILLION 1000000000.0

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
    // const char* LTE = "wwan0";
    // const char* WiFi = "wlan0";
    // const char* Actuator_IP_LTE = "10.20.0.13";      // Default: Actuator IP
    // const char* Actuator_IP_WiFi = "10.42.0.118";    // Default: Actuator IP (AP)
    // const char* Actuator_IP_LTE = "10.20.0.16";      // Control IP
    // const char* Actuator_IP_LTE = "10.20.0.10";      // Sensor IP
    // const char* Actuator_IP_WiFi = "192.168.1.143";  //Default: Actuator IP
    // const char* Actuator_IP_WiFi = "192.168.1.136";  // Control IP
    // const char* Actuator_IP_WiFi = "192.168.1.160";  // Sensor IP

    const char* LTE = "lo";                             // Test loopback
    const char* WiFi = "lo";                            // Test loopback
    const char* Actuator_IP_LTE = "127.0.0.1";          // Test loopback
    const char* Actuator_IP_WiFi = "127.0.0.1";         // Test loopback
    // const char* LTE = "enp0s3";             // Test loopback
    // const char* WiFi = "enp0s3";            // Test loopback
    // const char* Actuator_IP_LTE = "192.168.0.134";   // test loopback
    // const char* Actuator_IP_WiFi = "192.168.0.134";  // test loopback

    /* Misc */
    pthread_t T1;

    /* Struct for message & buffer size */
    char* msg;

    /* Execution time variables */
    struct timespec begin_program, end_program;
    unsigned long seconds = 0;
    unsigned long nanoseconds = 0;
    long double Execution_Average = 0;

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
    if (signal_monitor == 0 && monitor == 1) {
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
        sock.packet_count_LTE = 0;
        sock.packet_count_WiFi = 0;
        cout << "packet count LTE: " << sock.packet_count_LTE << endl;
        cout << "packet count WiFi: " << sock.packet_count_WiFi << endl;

        /* Start timing all code */
        clock_gettime(CLOCK_REALTIME, &begin_program);

        pthread_create(&T1, NULL, LTE_command, (void*)&sock);
        WiFi_command(&sock);

        cout << "NEW packet count LTE: " << sock.packet_count_LTE << endl;
        cout << "NEW packet count WiFi: " << sock.packet_count_WiFi << endl;
    }

    /* Stop timing all code */
    clock_gettime(CLOCK_REALTIME, &end_program);

    /* Calculation of total time execution */
    double time_spent = ((end_program.tv_sec - begin_program.tv_sec) +
                         (end_program.tv_nsec - begin_program.tv_nsec) / BILLION) -
                        5;

    /* Conversion of time spent to HH:MM:SS.MS */
    long hours = (long)time_spent / 3600;
    long minutes = ((long)time_spent / 60) % 60;
    long seconds2 = (long)time_spent % 60;
    long milliseconds = (long)(time_spent * 1000) % 1000;

    /* Calculation of execution average */
    Execution_Average = (sock.Execution_Sum_WiFi + sock.Execution_Sum_LTE) / (sock.packet_count_WiFi + sock.packet_count_LTE);

    printf("\n\n===================================\n\n");
    printf("Execution time sums:\n");
    printf("    Total Execution Sum:     %Lf sec\n", (sock.Execution_Sum_LTE + sock.Execution_Sum_WiFi));
    printf("     WiFi Execution Sum:     %Lf sec\n", sock.Execution_Sum_WiFi);
    printf("      LTE Execution Sum:     %Lf sec\n\n", sock.Execution_Sum_LTE);
    printf("Execution time average: \n");
    printf("    Total Execution average: %Lf sec\n\n", Execution_Average);
    printf("Total program time: \n");
    printf("    Total time: %f sec\n", time_spent);
    printf("________________________\n\n");
    printf("Total Time:  \n            Hours: %ld  \n          Minutes: %ld  \n          Seconds: %ld \n     Milliseconds: %ld\n", hours, minutes, seconds2, milliseconds);
    printf("________________________\n\n");
    printf("Total failed counts via WiFi:       %d\n", sock.fail_count_WiFi);
    printf("Total failed counts via LTE:        %d\n", sock.fail_count_LTE);
    printf("Total packets received via WiFi:    %d\n", sock.packet_count_WiFi);
    printf("Total packets received via LTE:     %d\n", sock.packet_count_LTE);
    printf("\n===================================\n\n");
}