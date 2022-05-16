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

#define buffer 10

pthread_t wifi, lte;

char* gsv = "0";  // Global Signal Variable   W = WiFi   L = LTE    B = Both
const char* GSV_KEY = "GSV_KEY";

/* Signal Quality Settings */
int wifi_rssi[buffer];
int rssi_sum;
int rssi_average;
int lte_rsrp[buffer];
int rsrp_sum;
int rsrp_average;

// WiFi
int rssi_bad = -80;
int rssi_mid = -70;
int rssi_good = -65;

// LTE
int rsrp_bad = -100;
int rsrp_mid = -85;
int rsrp_good = -80;

int main() {
    /* Initialize PORT & INTERFACE*/
    uint PORT_LTE = 9002;
    uint PORT_WiFi = 9003;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    /* Create sockets */
    Sockets sock;
    Sockets_Receiver(&sock, PORT_LTE, PORT_WiFi, LTE, WiFi);
    printf("sockLTE_RECEIVER: %d\n", sock.sockLTE_TRANSMITTER);
    printf("sockWiFi_RECEIVER: %d\n", sock.sockWiFi_TRANSMITTER);

    printf("==================\nMonitoring Process Started\n==================\n\n");
    int counter = 0;

    while (1) {
        wifi_rssi[counter] = RSSI_VAL();
        lte_rsrp[counter] = RSRP_VAL();

        if (gsv == "1" || gsv == "0") {  // If GSV is set to WiFi or both
            if (wifi_rssi[counter] < rssi_bad) {
                gsv = "2";
            }
        }

        if (gsv == "2" || gsv == "0") {
            if (lte_rsrp[counter] < rsrp_bad) {
                gsv = "1";
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
            gsv = "1";  // If WiFi has stronger signal, set WiFi
            // printf("GSV: WiFi Selected\n");
        } else if ((rssi_average < rssi_good && rsrp_average >= rsrp_good) || (rssi_average < rssi_mid && rsrp_average >= rsrp_mid)) {
            gsv = "2";  // If LTE has stronger signal, set LTE
            // printf("GSV: LTE Selected\n");
        }
        // else if( (rssi_average >= rssi_good && rsrp_average >= rsrp_good) || (rssi_average >= rssi_mid && rsrp_average >= rsrp_mid) ) {
        else {
            gsv = "0";  // If no clear winner, set to send on both
            printf("GSV: Both Selected\n");
        }
        shm_write(gsv, buffer, GSV_KEY);  // Write selected technology to shared memory

        if (gsv == "1" || gsv == "0") {
            pthread_t wifi, lte;
            Timestamp();
            int threadWiFi = pthread_create(&wifi, NULL, transmitWiFi, (void*)&sock);
            // pthread_join(wifi, NULL);
            if (threadWiFi == 0) {
                // printf("WiFi thread is running!\n");
            } else {
                perror("WiFi thread was not created");
            }
            // printf("GSV: Sent via WiFi\n");
        }
        if (gsv == "2" || gsv == "0") {
            Timestamp();
            int threadLTE = pthread_create(&lte, NULL, transmitLTE, (void*)&sock);
            // pthread_join(lte, NULL);
            if (threadLTE == 0) {
                // printf("LTE thread is running!\n");
            } else {
                perror("LTE thread was not created");
            }
            // printf("GSV: Sent via LTE\n");
        }

        if (counter == (buffer - 1)) {
            counter = 0;
        } else {
            counter++;
        }

        sleep(1);
    }
}