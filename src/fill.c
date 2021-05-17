#include "../includes/ft_malcolm.h"

int check_index(){
	struct ifaddrs *ifap, *ifa;
	int i = -1;

	if (getifaddrs(&ifap) != 0){
		perror("getifaddrs()");
		return (0);
	}
	for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next){
		i++;
		if (ft_strncmp(ifa->ifa_name, "lo", 2) == 0)
			continue ;
		break ;
	}
	freeifaddrs(ifap);
	return (i);
}

void fill_arphdr(arp_hdr *arphdr, char **av){
	char **arp_sha, **arp_spa, **arp_tha, **arp_tpa;
	int i;

	arp_spa = ft_strsplit(av[1], '.');
	arp_tpa = ft_strsplit(av[3], '.');
	arp_sha = ft_strsplit(av[2], ':');
	arp_tha = ft_strsplit(av[4], ':');
	arphdr->htype = htons(1);
	arphdr->ptype = htons(ETH_P_IP);
	arphdr->hlen = 6;
	arphdr->plen = 4;
	arphdr->opcode = htons(ARPOP_REPLY);
	for (i = 0; i < MAC_ADDR_LEN; i++)
		arphdr->sender_mac[i] = hex_simple(arp_sha[i]);
	for (i = 0; i < IP_ADDR_LEN; i++)
		arphdr->sender_ip[i] = ft_atoi(arp_spa[i]);
	for (i = 0; i < MAC_ADDR_LEN; i++)
		arphdr->target_mac[i] = hex_simple(arp_tha[i]);
	for (i = 0; i < IP_ADDR_LEN; i++)
		arphdr->target_ip[i] = ft_atoi(arp_tpa[i]);
}

void fill_device(struct sockaddr_ll *device, uint8_t addr[]){
	device->sll_family = AF_PACKET;
//	device->sll_protocol = htons(ETH_P_ARP);
	device->sll_ifindex = check_index();
//	device->sll_hatype = ARPHRD_ETHER;
//    device->sll_pkttype = 0; //PACKET_OTHERHOST
	device->sll_halen = htons(6);
	memcpy(device->sll_addr, addr, 6 * sizeof (uint8_t));
	printf("sll_ifindex = %d\n", device->sll_ifindex);
}
