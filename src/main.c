#include "../includes/ft_malcolm.h"

int	main(int ac, char **av){
	struct ifaddrs *ifaddr, *ifa;
    int family, n;
//    char host[NI_MAXHOST];

	if (ac != 5) {
        printf("Usage: ./ft_malcolm <source IP> <source mac address> <target IP> <target mac address>\n");
        return EXIT_FAILURE;
	}
    else{
		(void)av;
		if (getuid() != 0){
		    printf("Usage: ./ft_malcolm <source IP> <source mac address> <target IP> <target mac address> \nYou must be root or sudo user\n");
		    return EXIT_FAILURE;
		}
		if (getifaddrs(&ifaddr) == -1)
			return EXIT_FAILURE;
		for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++){
            if (ifa->ifa_addr == NULL)
                continue;
            family = ifa->ifa_addr->sa_family;
//            printf("%s\n", ifa->ifa_name);
            printf("%-8s %s (%d)\n",
                   ifa->ifa_name,
                   (family == AF_PACKET) ? "AF_PACKET" :
                   (family == AF_INET) ? "AF_INET" :
                   (family == AF_INET6) ? "AF_INET6" : "???",
                   family);
		}
		return EXIT_SUCCESS;
	}
}
