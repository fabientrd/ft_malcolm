#include "../includes/ft_malcolm.h"

uint8_t hex_simple(char *s){
	uint8_t ret1;
	uint8_t ret2;

	switch (s[0]){
		case('a') : ret1 = 10 * 16; break ;
		case('b') : ret1 = 11 * 16; break ;
		case('c') : ret1 = 12 * 16; break ;
		case('d') : ret1 = 13 * 16; break ;
		case('e') : ret1 = 14 * 16; break ;
		case('f') : ret1 = 15 * 16; break ;
		default : ret1 = ((int)s[0] - 48) * 16;
	}
	switch (s[1]){
		case('a') : ret2 = 10; break ;
		case('b') : ret2 =  11; break ;
		case('c') : ret2 = 12; break ;
		case('d') : ret2 = 13; break ;
		case('e') : ret2 = 14; break ;
		case('f') : ret2 = 15; break ;
		default : ret2 = (int)s[1] - 48;
	}
	return (ret1 + ret2);
}

void fill_arphdr(arp_hdr *arphdr, char **av){
	(void)av;
	char **arp_sha, **arp_spa, **arp_tha, **arp_tpa;
	int i;

	arp_spa = ft_strsplit(av[1], '.');
	arp_tpa = ft_strsplit(av[3], '.');
	arp_sha = ft_strsplit(av[2], ':');	arp_tha = ft_strsplit(av[4], ':');
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

int check_up_target(char *s){
(void)s;
	return (0);
}

int arp_reply(char **av){
	int sock;
//	struct in_addr src_in_addr, targ_in_addr;
	arp_hdr arphdr;
//	uint8_t src_ip[4], src_mac[6], dst_mac[6], ether_frame[IP_MAXPACKET];
//	struct ether_arp*arp_packet = NULL;
//	struct sockaddr sa;

	printf("Initialization of the ARP reply\nCheck if the target IP (%s) is up, please wait ...\n", av[3]);
	if (check_up_target(av[3]) != 0){
		printf("Unknown host IP: %s\n", av[3]);
		return (-1);
	}
	sleep(1);
	sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if(sock < 0){
		perror("socket()");
		return (-1);
	}
	fill_arphdr(&arphdr, av);
	return (0);
}
