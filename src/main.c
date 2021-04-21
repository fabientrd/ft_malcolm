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
    }
    else if (x == 2 && y == 0 && z == 0){
        if (ft_strlen(s) != 14)
            return (-1);
        for (i = 0; s[i]; i++){
            if (i == 4 || i == 9)
                continue ;
            if (s[i] <= 47 || (s[i] >= 58 && s[i] <= 96) || s[i] >= 103)
                return (-1);
        }
        return (0);
    }
    else
        return (-1);
}

int check_valid_IPv4(char *s) {
    struct ifaddrs *ifaddr, *ifa;
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
                if (check_valid_IPv4(av[i]) != 0) {
                    i == 1 ? printf("ft_malcolm: unknown source host address: (%s)\n", av[i]) : printf(
                            "ft_malcolm: unknown target host address: (%s)\n", av[i]);
                    return (-1);
                }
            }
        } else {
            printf("av[i] = %s\n", av[i]);
            if (check_MAC_format(av[i]) != 0) {
                i == 2 ? printf("ft_malcolm: invalid source MAC address: (%s)\n", av[i]) : printf(
                        "ft_malcolm: invalid target MAC address: (%s)\n", av[i]);
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
        (void) av;
        if (getuid() != 0) {
            printf("Usage: ./ft_malcolm <source IP> <source mac address> <target IP> <target mac address>"
                   " \nYou must be root or sudo user\n");
            return EXIT_FAILURE;
        }
        if (check_args(av) != 0) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
