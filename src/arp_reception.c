#include "../includes/ft_malcolm.h"

void die(char *s)
{
	perror(s);
	exit(1);
}

/*int	init_socket(){
//WAITING FOR ARP REQUEST
	int ret;
	int broadcast = 1;
	struct sockaddr_in si_me, si_other;
	char buf[BUF_LEN];
	int s, slen = sizeof(si_other), recvlen;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == -1){
		perror("socket");
		close(s);
		return (-1);
	}
	else{
		ret = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
		if (ret == -1){
			perror("setsockopt");
			close(s);
			return (-1);
		}
		memset((char *) &si_me, 0, sizeof(si_me));
		si_me.sin_family = AF_INET;
		si_me.sin_port = htons(PORT);
		si_me.sin_addr.s_addr = htonl(INADDR_ANY);
		if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
			perror("bind");
		while (1){
		printf("Waiting for data...");
		fflush(stdout);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (int)&slen)) == -1)
			perror("recvfrom()");
		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: %s\n" , buf);
		}
	}	
	close(s);
	return (0);

struct sockaddr_in si_me, si_other;

	int s, recv_len;
	socklen_t slen = sizeof(si_other);
	char buf[BUFLEN];

	//create a UDP socket
	if ((s=socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}
	on=1;
	if(setsockopt(s,SOL_SOCKET,SO_BROADCAST,&on,sizeof(int))==-1)
	{
		perror("setsockopt SO_BROADCAST");
		close(s);
		return -1;
	}

	// zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));

	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htons(INADDR_ANY);

	//bind socket to port
	if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
	{
		die("bind");
	}

	//keep listening for data
	while(1)
	{
		printf("Waiting for data...");
		fflush(stdout);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
		{
			die("recvfrom()");
		}

		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: %s\n" , buf);

		//now reply the client with the same data
		if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
		{
			die("sendto()");
		}
	}

	close(s);
	return 0;

}*/

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
	int					sock_raw_fd, recv_len, i;
//	char ip_src[512];
//
	if (!(finding_interface())){
		printf("No available interfaces found\nExiting !\n");
		return (-1);
	}
	if ((sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
		perror("socket()");
	printf("Waiting for data ...\n");
	while (1)
	{
		bzero(buf, ETHER_ARP_PACKET_LEN);
		recv_len = recvfrom(sock_raw_fd, buf,  ETHER_ARP_PACKET_LEN, 0, 0, 0);
		if (recv_len > 0)
		{
			/*  Strip off the ether head */
			arp_packet = (struct ether_arp *)(buf + ETHER_HEADER_LEN);
			/*  arp opcode 1 means arp request */
			if (ntohs(arp_packet->arp_op) == 1)
			{
				if (arp_packet->arp_spa[3] != 29)
					continue ;
				printf("An ARP Request has been broadcast on the network.\n");
				printf("	IP address of the broadcast: ");
				for (i = 0; i < IP_ADDR_LEN; i++){
					printf("%u", arp_packet->arp_spa[i]);
					i + 1 == IP_ADDR_LEN ? printf("\n") : printf(".");
				}
				printf("	MAC address of request: ");
				for (i = 0; i < ETH_ALEN; i++){
					printf("%02x", arp_packet->arp_sha[i]);
					i + 1 == ETHER_ADDR_LEN ? printf("\n") : printf(":");
				}
				printf("Now sending an ARP reply to the target address with spoof source, please wait ...\n");
				break ;
			}
			
		}
	}
	close(sock_raw_fd);
	return (0);

}
