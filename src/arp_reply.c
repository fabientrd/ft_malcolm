#include "../includes/ft_malcolm.h"

int check_up_target(char *s){
	printf("target IP address is : %s\n", s);
	return (0);
}

int arp_reply(char **av){

	char mac[ETHER_ADDR_LEN * 3];
	struct hostent *hostinfo;

	hostinfo = gethostbyname(av[3]);
	if (hostinfo == NULL)
		printf("Unknown host IP: %s\n", av[3]);

	(void)mac;
	printf("Initialization of the ARP reply\nCheck if the target IP (%s) is up, please wait ...\n", av[3]);
	if (check_up_target(av[3]) != 0){
		printf("Unknown host IP: %s\n", av[3]);
		return (-1);
	}
	return (0);
}
