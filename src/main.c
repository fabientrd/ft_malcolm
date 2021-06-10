#include "../includes/ft_malcolm.h"
#define _GNU_SOURCE     /* Afin d'avoir accès aux définitions de NI_MAXSERV et de  NI_MAXHOST */
       #include <arpa/inet.h>
       #include <sys/socket.h>
       #include <netdb.h>
       #include <ifaddrs.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>
       #include <linux/if_link.h>

int check_MAC_format(char *s) {
    int i = 0;
    int x = 0;
    int y = 0;
    int z = 0;
    while (s[i]) {
        s[i] == '.' ? x += 1 : x;
        s[i] == '-' ? y += 1 : y;
        s[i] == ':' ? z += 1 : z;
        i++;
    }
    if ((y == 5 && z == 0 && x == 0) || (z == 5 && y == 0 && x == 0)) {
        if (ft_strlen(s) != 17)
            return (-1);
        for (i = 0; s[i]; i++) {
            if (i == 2 || i == 5 || i == 8 || i == 11 || i == 14)
                continue;
            if (s[i] <= 47 || (s[i] >= 58 && s[i] <= 96) || s[i] >= 103)
                return (-1);
        }
        return (0);
    } else if (x == 2 && y == 0 && z == 0) {
        if (ft_strlen(s) != 14)
            return (-1);
        for (i = 0; s[i]; i++) {
            if (i == 4 || i == 9)
                continue;
            if (s[i] <= 47 || (s[i] >= 58 && s[i] <= 96) || s[i] >= 103)
                return (-1);
        }
        return (0);
    } else
        return (-1);
}

int check_valid_IPv4(char *str) {
   /* struct ifaddrs *ifaddr, *ifa;
    int family;
    (void) s;

    if (getifaddrs(&ifaddr) == -1)
        return EXIT_FAILURE;
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;
        family = ifa->ifa_addr->sa_family;
    }
    freeifaddrs(ifaddr);
    return (0);
    */
	(void)str;
    struct ifaddrs *ifaddr, *ifa;
           int family, s, n;
           char host[NI_MAXHOST];

           if (getifaddrs(&ifaddr) == -1) {
               perror("getifaddrs");
               exit(EXIT_FAILURE);
           }

           /* Walk through linked list, maintaining head pointer so we
              can free list later */

           for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
               if (ifa->ifa_addr == NULL)
                   continue;

               family = ifa->ifa_addr->sa_family;

               /* Display interface name and family (including symbolic
                  form of the latter for the common families) */

               printf("%-8s %s (%d)\n",
                      ifa->ifa_name,
                      (family == AF_PACKET) ? "AF_PACKET" :
                      (family == AF_INET) ? "AF_INET" :
                      (family == AF_INET6) ? "AF_INET6" : "???",
                      family);

               /* For an AF_INET* interface address, display the address */

               if (family == AF_INET || family == AF_INET6) {
                   s = getnameinfo(ifa->ifa_addr,
                           (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                           host, NI_MAXHOST,
                           NULL, 0, NI_NUMERICHOST);
                   if (s != 0) {
                       printf("getnameinfo() failed: %s\n", gai_strerror(s));
                       exit(EXIT_FAILURE);
                   }

                   printf("\t\taddress: <%s>\n", host);

               } else if (family == AF_PACKET && ifa->ifa_data != NULL) {
                   struct rtnl_link_stats *stats = ifa->ifa_data;

                   printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
                          "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
                          stats->tx_packets, stats->rx_packets,
                          stats->tx_bytes, stats->rx_bytes);
               }
           }

           freeifaddrs(ifaddr);
           exit(EXIT_SUCCESS);
}

int check_args(char **av) {
    struct hostent *hostent;

    for (int i = 1; av[i]; i++) {
        if (i == 1 || i == 3) {
            if ((hostent = gethostbyname(av[i])) == NULL) {
                i == 1 ? printf("ft_malcolm: invalid source IP address: (%s)\n", av[i]) : printf(
                        "ft_malcolm: invalid target IP adress: (%s)\n", av[i]);
                return (-1);
            } else {
              /*  if (check_valid_IPv4(av[i]) != 0) {
                    i == 1 ? printf("ft_malcolm: unknown source host address: (%s)\n", av[i]) : printf(
                            "ft_malcolm: unknown target host address: (%s)\n", av[i]);
                    return (-1);
                } */
	    }
        } else {
            if (check_MAC_format(av[i]) != 0) {
                i == 2 ? printf("ft_malcolm: invalid source MAC address: (%s)\n", av[i]) : printf("ft_malcolm: invalid target MAC address: (%s)\n", av[i]);
                return (-1);
            }
        }
    }
    return (0);
}


int main(int ac, char **av) {
    if (ac != 5) {
        printf("Usage: ./ft_malcolm <source IP> <source mac address> <target IP> <target mac address>\n");
        return EXIT_FAILURE;
    } else {
        if (getuid() != 0) {
            printf("Usage: ./ft_malcolm <source IP> <source mac address> <target IP> <target mac address>"
                   " \nYou must be root or sudo user\n");
            return EXIT_FAILURE;
        }
        if (check_args(av) != 0) {
            return EXIT_FAILURE;
        }
		printf("Initializing Man In The Middle Attack\n");
		sleep(1);
		if (arp_reception() != 0){
			printf("Something went wrong during the ARP reception\n");
			return EXIT_FAILURE;
		}
		if (arp_reply(av) != 0){
			printf("Something went wrong during the ARP reply\n");
			return EXIT_FAILURE;
		}
    }
    return EXIT_SUCCESS;
}
