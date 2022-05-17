#ifndef LIBRARIES 
#define LIBRARIES
#include "Libraries.cpp"      // File with all our includes
#endif

#ifndef SHM_WRITE
#define SHM_WRITE

// #include "Headers/shm_write_read.h"
#include "Headers/shm_write.cpp"

#endif

#ifndef SOCKETS_MON
#define SOCKETS_MON
#include "Headers/Sockets.h"
#endif
#include "Headers/Sockets.h"

#define buffer 10

pthread_t wifi, lte;

// Both = 0     WiFi = 1        LTE = 2
char* gsv =  (char*) "0";  // Global Signal Variable   W = WiFi   L = LTE    B = Both
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
    uint PORT_LTE = 0;
    uint PORT_WiFi = 0;
    uint PORT_LTE_TRANS = 9002;
    uint PORT_WiFi_TRANS = 9003;
    const char* LTE = "wwan0";
    const char* WiFi = "wlan0";

    /* Create sockets */
    Sockets sock;
    Create_Bind_Sockets(&sock, PORT_LTE, PORT_WiFi, PORT_LTE_TRANS, PORT_WiFi_TRANS, LTE, WiFi);

    printf("==================\nMonitoring Process Started\n==================\n\n");
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
            printf("GSV: WiFi Selected\n");
        } else if ((rssi_average < rssi_good && rsrp_average >= rsrp_good) || (rssi_average < rssi_mid && rsrp_average >= rsrp_mid)) {
            gsv =  (char*)"2";  // If LTE has stronger signal, set LTE
            printf("GSV: LTE Selected\n");
        }
        // else if( (rssi_average >= rssi_good && rsrp_average >= rsrp_good) || (rssi_average >= rssi_mid && rsrp_average >= rsrp_mid) ) {
        else {
            gsv =  (char*)"0";  // If no clear winner, set to send on both
            printf("GSV: Both Selected\n");
        }
        shm_write(gsv, buffer, GSV_KEY);  // Write selected technology to shared memory

        if (gsv == "1" || gsv == "0") {
            pthread_t wifi, lte;
            int threadWiFi = pthread_create(&wifi, NULL, transmitWiFi, (void*)&sock);
            //pthread_join(wifi, NULL);
            if (threadWiFi == 0) {
                printf("WiFi thread is running!\n");
            } else {
                perror("WiFi thread was not created");
            }
            printf("GSV: Sent via WiFi\n");
        }
        if (gsv == "2" || gsv == "0") {
            int threadLTE = pthread_create(&lte, NULL, transmitLTE, (void*)&sock);
            //pthread_join(lte, NULL);
            if (threadLTE == 0) {
                printf("LTE thread is running!\n");
            } else {
                perror("LTE thread was not created");
            }
            printf("GSV: Sent via LTE\n");
        }

        if (counter == (buffer - 1)) {
            counter = 0;
        } else {
            counter++;
        }

        sleep(1);
    }
}