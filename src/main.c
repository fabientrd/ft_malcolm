#include "../includes/ft_malcolm.h"

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

int	compare_subnet(char *s1, char *s2){
	char **cmp1, **cmp2;
	int i;

	cmp1 = ft_strsplit(s1, '.');
	cmp2 = ft_strsplit(s2, '.');
	for (i = 0; i <= 2; i++){
		if (ft_strcmp(cmp1[i], cmp2[i])){
			free_subnet(cmp1, cmp2);
			return (1);
		}
	}
	free_subnet(cmp1, cmp2);
	return (0);
}

int check_subnet(char *av){
	struct ifaddrs *ifap, *ifa;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifap) != 0){
		perror("getifaddrs()");
		return (1);
	}
	for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next){
		if (ifa->ifa_addr->sa_family == AF_INET){
			getnameinfo(ifa->ifa_addr, (ifa->ifa_addr->sa_family == AF_INET) 
			? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), host,
			NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			if (ft_strncmp(ifa->ifa_name, "lo", 2)){
				if (compare_subnet(av, host)){
					freeifaddrs(ifap);
					return (1);
				}
			}
		}
	}
	freeifaddrs(ifap);
	return (0);
}

int check_args(char **av) {
    struct hostent *hostent;
//	int ret;
    
	for (int i = 1; av[i]; i++) {
        if (i == 1 || i == 3) {
            if ((hostent = gethostbyname(av[i])) == NULL) {
					i == 1 ? printf("ft_malcolm: invalid source IP address:"
					"(%s)\n" , av[i]) : printf("ft_malcolm: invalid target IP"
					" address: (%s)\n", av[i]);
				return (-1);
            }
			if (check_subnet(av[i])){
				i == 1 ? printf("ft_malcolm: source IP address doesn't match"
				" with the subnet\n") : printf("ft_malcolm: target IP address"
				" doesn't match with the subnet\n") ;
				return (-1);
			}
        } else {
            if (check_MAC_format(av[i]) != 0) {
                i == 2 ? printf("ft_malcolm: invalid source MAC address: (%s)"
				"\n", av[i]) : printf("ft_malcolm: invalid target MAC address:"
				"(%s)\n", av[i]);
                return (-1);
            }
        }
    }
    return (0);
}


int main(int ac, char **av) {
	int ret;

    if (ac != 5) {
        printf("Usage: ./ft_malcolm <source IP> <source mac address>"
		" <target IP> <target mac address>\n");
        return EXIT_FAILURE;
    } else {
        if (getuid() != 0) {
            printf("Usage: ./ft_malcolm <source IP> <source mac address> <targ"
			"et IP> <target mac address> \nYou must be root or sudo user\n");
            return EXIT_FAILURE;
        }
        if (check_args(av) != 0) {
            return EXIT_FAILURE;
        }
		printf("Initializing Man In The Middle Attack\n");
		sleep(1);
		if ((ret = arp_reception()) != 0){
			if (ret == 1)
				return EXIT_SUCCESS;
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
