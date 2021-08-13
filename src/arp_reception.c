#include "../includes/ft_malcolm.h"

int inter;

void handler(){
	printf("\nThe program has been stopped correctly ... Exiting\n");
	inter = 1;
	return ;
}

void die(char *s)
{
	perror(s);
	exit(1);
}

int	finding_interface(){
	
	struct ifaddrs *ifap, *ifa;

	if (getifaddrs(&ifap) != 0){
		perror("getifaddrs()");
		return (0);
	}
	for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next){
		if (ft_strncmp(ifa->ifa_name, "lo", 2) == 0)
			continue ;
		printf("Found available interface: %s\n", ifa->ifa_name);
		break ;
	}
	freeifaddrs(ifap);
	return (1);
}



int arp_reception(){
	struct ether_arp	*arp_packet;
	char				buf[ETHER_ARP_PACKET_LEN];
	int					sock_raw_fd, recv_len;
	struct				timeval read_timeout;
//
	if (!(finding_interface())){
		printf("No available interfaces found\nExiting !\n");
		return (-1);
	}
	if ((sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
		perror("socket()");
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;
	setsockopt(sock_raw_fd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
	printf("Waiting for data ...\n");
	signal(SIGINT, handler);
	while (inter == 0)
	{
		bzero(buf, ETHER_ARP_PACKET_LEN);
		recv_len = recvfrom(sock_raw_fd, buf, ETHER_ARP_PACKET_LEN, 0, 0, 0);
		if (recv_len > 0)
		{
			/*  Strip off the ether head */
			arp_packet = (struct ether_arp *)(buf + ETHER_HEADER_LEN);
			/*  arp opcode 1 means arp request */
			if (ntohs(arp_packet->arp_op) == 1)
			{
				display((unsigned char *)buf);
				printf("Now sending an ARP reply to the target address with spoof source, please wait ...\n");
				break ;
			}
			
		}
	}
	close(sock_raw_fd);
	if (inter == 1)
		return (1);
	return (0);

}
