
    #include <string.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <ifaddrs.h>
    #include <sys/types.h>
    #include <sys/ioctl.h>
    #include <sys/socket.h>
    #include <linux/wireless.h>
    #include <stdlib.h>
    #include <errno.h>

    char ifname[IFNAMSIZ];
    char essid[IW_ESSID_MAX_SIZE+1];
    struct iwreq wrq;
    struct iw_statistics *iwstats;
    int sock;


    int findWifi() {
      struct ifaddrs *ifaddr, *ifa;

      if (getifaddrs(&ifaddr) == -1) {
        perror("Error getting interface addresses");
        return -1;
      }

      for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        strncpy(wrq.ifr_name, ifa->ifa_name, IFNAMSIZ);

        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
          perror("Error opening INET socket");
        }

        if (ioctl(sock, SIOCGIWNAME, &wrq) == 0) {
          printf("IF: %s\n", wrq.ifr_name);
          strcpy(ifname, wrq.ifr_name);
          freeifaddrs(ifaddr);
          return sock;
        }
      }
      freeifaddrs(ifaddr);
      return 0;
    }

    int wifiInfo() {
      wrq.u.essid.pointer = essid;
      if (ioctl(sock, SIOCGIWESSID, &wrq) == -1) {
        perror("Can't open socket to obtain essid");
        return(-1);
      }

      printf("ESSID is %p\n", wrq.u.essid.pointer);

      memset(&iwstats, 0, sizeof(iwstats));

      wrq.u.data.pointer = (caddr_t) &iwstats;
      wrq.u.data.length = sizeof(&iwstats);
      wrq.u.data.flags = 1;

      // ERROR HERE...
      if (ioctl(sock, SIOCGIWSTATS, &wrq) == -1) {
        perror("Can't open socket to obtain iwstats");
        return(-1);
      }

      return(0);
    }

    int main(int argc, char const *argv[]) {
      findWifi();
      wifiInfo();
      return 0;
    }