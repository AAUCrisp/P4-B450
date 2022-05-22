#ifndef LIBRARIES 
#define LIBRARIES
#include "Libraries.cpp"      // File with all our includes
#endif

#ifndef SHM_WRITE
#define SHM_WRITE
#include "Headers/shm_write.cpp"
#endif

#ifndef SOCKETS_MON
#define SOCKETS_MON
#include "Headers/Sockets.h"
// #include "Headers/Sockets.cpp"
// #include "Headers/RSSI.cpp"
#endif

#define buffer 32


pthread_t wifi, lte;

// Both = 0     WiFi = 1        LTE = 2
char* gsv =  (char*) "0";  // Global Signal Variable   W = WiFi   L = LTE    B = Both
const char* GSV_KEY = "GSV_KEY";

/* Signal Quality Settings */

// WiFi
int rssi_bad = -80;
int rssi_mid = -70;
int rssi_good = -65;
int wifi_rssi[buffer];
int rssi_sum;
int rssi_average;

// LTE
int rsrp_bad = -100;
int rsrp_mid = -90;
int rsrp_good = -80;
int lte_rsrp[buffer];
int rsrp_sum;
int rsrp_average;


int main(int argc, char *argv[]) {

    // If Arguments is inserted
    if(argc > 1) {      // If the program is run with arguments
        int child = 0;

        // If Child Process
        if((string) argv[1] == "-a") {
            child = 1;
        }
        else {
            cout << "Sensor Monitoring Arguments Received" << endl;
        }
        for (int i = 1; i < argc; i++) {

            // Force Technologies Argument
            if((string) argv[i] == "-t" || (string) argv[i] == "-tech" || (string) argv[i] == "-technology") {

                if( argc > i+1) {
                    string current = argv[i+1];
                    char firstCharacter = current.at(0);

                    if( (firstCharacter == '-') || (string) argv[i+1] == "b" ||  (string) argv[i+1] == "both") {
                        if(child == 0) {
                            cout << "  ===== Forced Use of Both Technologies Enabled =====" << endl;
                        }
                        force_tech = 1;
                    }
                    else if((string) argv[i+1] == "w" || (string) argv[i+1] == "wifi") {
                        if(child == 0) {
                            cout << "  ===== Forced Use of WiFi =====" << endl;
                        }
                        force_tech = 2;
                    }
                    else if ((string) argv[i+1] == "l" || (string) argv[i+1] == "lte") {
                        if(child == 0) {
                            cout << "  ===== Forced Use of LTE =====" << endl;
                        }
                        force_tech = 3;
                    } 
                }
                else {
                    if(child == 0) {
                        cout << "  ===== Forced Use of Both Technologies Enabled =====" << endl;
                    }
                    force_tech = 1;
                }
            }

            // Verbose Argument
            if((string) argv[i] == "-v" || (string) argv[i] == "-verbose") {
                if(child == 0) {
                    cout << "===== Verbose Enabled =====" << endl;
                }
                message_only = 0;        // Print messages only
                troubleshooting_print = 1;
                print_GSV = 1;          // Print GSV related things
            }
            
            // Sleep Argument
            if((string) argv[i] == "-s" || (string) argv[i] == "-sleep") {
                delay = atoi(argv[i+1]);
                if(child == 0) {
                    cout << "===== Sleep Time Changed to " << delay << " seconds =====" << endl;
                }
            }
        }
    }

    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE_TRANSMITTER = 9002;
    uint PORT_WiFi_TRANSMITTER = 9003;
    const char* LTE = "wwan0"; // Interface for 4G
    const char* WiFi = "wlan0"; // Interface for WiFi
    const char* IP_LTE = "10.20.0.10"; // IP of server
    const char* IP_WiFi = "192.168.1.160"; // IP of server 
    pthread_t wifi, lte;

    /* Create sockets */
    Sockets sock;
    Sockets_GSV(&sock, IP_LTE, IP_WiFi, PORT_LTE_TRANSMITTER, PORT_WiFi_TRANSMITTER, LTE, WiFi);

    printf("\n\n  ============================\n   Monitoring Process Started\n  ============================\n\n");
    int counter = 0;

    while (1) {
        wifi_rssi[counter] = RSSI_VAL();
        lte_rsrp[counter] = RSRP_VAL();

        if (gsv == "1" || gsv == "0") {  // If GSV is set to WiFi or both
            if (wifi_rssi[counter] < rssi_bad) {
                gsv =  (char*)"2";
            }
        }

        if (gsv == "2" || gsv == "0") {
            if (lte_rsrp[counter] < rsrp_bad) {
                gsv =  (char*)"1";
            }
        }

        rssi_sum = 0;
        rsrp_sum = 0;

        for (int i = 0; i < sizeof(buffer); i++) {
            rssi_sum += wifi_rssi[i];
            rsrp_sum += lte_rsrp[i];
        }

        rssi_average = rssi_sum / sizeof(buffer);
        rsrp_average = rsrp_sum / sizeof(buffer);

        /* Compare signals and select a technology */
        if ((rssi_average >= rssi_good && rsrp_average < rsrp_good) || (rssi_average >= rssi_mid && rsrp_average < rsrp_mid)) {
            gsv =  (char*)"1";  // If WiFi has stronger signal, set WiFi
            if(troubleshooting_print == 1 || message_only == 1) {
                printf("GSV || WiFi Selected\n\n");
            }
        } 
        else if ((rssi_average < rssi_good && rsrp_average >= rsrp_good) || (rssi_average < rssi_mid && rsrp_average >= rsrp_mid)) {
            gsv =  (char*)"2";  // If LTE has stronger signal, set LTE
            if(troubleshooting_print == 1 || message_only == 1) {
                printf("GSV || LTE Selected\n\n");
            }
        }
        else {
            gsv =  (char*)"0";  // If no clear winner, set to send on both
            if(troubleshooting_print == 1 || message_only == 1) {
                printf("GSV || Both Selected\n\n");
            }
        }

        if(force_tech == 1) {
            gsv = (char*)"0";   // Force both technologies if enabled
        }
        shm_write(gsv, buffer, GSV_KEY);  // Write selected technology to shared memory

        if (gsv == "1" || gsv == "0") {
            int threadWiFi = pthread_create(&wifi, NULL, transmit_GSV_WiFi, (void*)&sock);

            if (threadWiFi != 0) {
                perror("GSV || WiFi thread was not created");
            } 
            if(troubleshooting_print == 1) {
                printf("GSV || Sent via WiFi\n");
            }
        }
        if (gsv == "2" || gsv == "0") {
            int threadLTE = pthread_create(&lte, NULL, transmit_GSV_LTE, (void*)&sock);
            if (threadLTE != 0) {
                perror("GSV || LTE thread was not created");
            }
            if(troubleshooting_print == 1) {
                printf("GSV || Sent via LTE\n");
            }
        }

        if (counter == (buffer - 1)) {
            counter = 0;
        } else {
            counter++;
        }

        sleep(delay);
    }
}