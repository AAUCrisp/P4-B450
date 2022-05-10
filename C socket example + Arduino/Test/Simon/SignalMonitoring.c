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
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "Sockets_TX_RX.h"
#include "shm_write_read.h"

#define buffer 10

Sockets sock;

pthread_t wifi, lte;

char* gsv = "B";        // Global Signal Variable   W = WiFi   L = LTE    B = Both
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

    int counter = 0;

    while(1){
        wifi_rssi[counter] = RSSI_VAL();
        lte_rsrp[counter] = RSRP_VAL();


        if(gsv == "W" || gsv == "B") {    // If GSV is set to WiFi
            if(wifi_rssi[counter] < rssi_bad){
                gsv = "L";
            }
        }

        if(gsv == "L" || gsv == "B") {
            if(lte_rsrp[counter] < rsrp_bad) {
                gsv = "W";
            }
        }

        rssi_sum = 0;
        rsrp_sum = 0;

        for(int i = 0; i < sizeof(buffer); i++){
            rssi_sum += wifi_rssi[i];
			rsrp_sum += lte_rsrp[i];
        }

		rssi_average = rssi_sum / sizeof(buffer);
		rsrp_average = rsrp_sum / sizeof(buffer);


		/* Compare signals and select a technology */
		if( (rssi_average >= rssi_good && rsrp_average >= rsrp_good) || (rssi_average >= rssi_mid && rsrp_average >= rsrp_mid) ) {
			gsv = "B";		// If no clear winner, set to send on both
		}
		else if( (rssi_average >= rssi_good && rsrp_average < rsrp_good) || (rssi_average >= rssi_mid && rsrp_average < rsrp_mid) ) {
			gsv = "W";		// If WiFi has stronger signal, set WiFi
		}
		else if( (rssi_average < rssi_good && rsrp_average >= rsrp_good) || (rssi_average < rssi_mid && rsrp_average >= rsrp_mid) ) {
			gsv = "L";		// If LTE has stronger signal, set LTE
		}
		shm_write(gsv, buffer, GSV_KEY);	// Write selected technology to shared memory


		if(gsv == "W" || gsv == "B") {
			pthread_create(&wifi, NULL, transmitWiFi, (void*) &sock);
		}
		if(gsv == "L" || gsv == "B") {
			pthread_create(&lte, NULL, transmitLTE, (void*) &sock);
		}

        if(counter == (buffer -1)) { counter = 0; }
		else{ counter++;}
    }
}