#include "../includes/ft_malcolm.h"

void display(unsigned char *frame){
	frame[21] == 2 ? printf("\nAn ARP Reply has been sent.\nPacket Informations:\n") : printf("An ARP Request has been broadcast on the network.\nPacket Informations:\n");
	printf("	Destination: %02x:%02x:%02x:%02x:%02x:%02x\n", frame[0], frame[1], frame[2], frame[3], frame[4], frame[5]);
	printf("	Source: %02x:%02x:%02x:%02x:%02x:%02x\n", frame[6], frame[7], frame[8], frame[9], frame[10], frame[11]);
	printf("	Type: 0x%02x%02x\n", frame[12], frame[13]);
	printf("	Hardware Type: %d\n", frame[15]);
	printf("	Protocol Type: 0x%02x%02x\n", frame[16], frame[17]);
	printf("	Hardware Size: %d\n	Protocol Size: %d\n	Opcode: %d ", frame[18], frame[19], frame[21]);
	frame[21] == 1 ? printf("(request)\n") : printf("(reply)\n");
	printf("	MAC address of source: %02x:%02x:%02x:%02x:%02x:%02x\n", frame[22], frame[23], frame[24], frame[25], frame[26], frame[27]);
	printf("	IP address of source: %d.%d.%d.%d\n", frame[28], frame[29], frame[30], frame[31]);
	printf("	MAC address of target: %02x:%02x:%02x:%02x:%02x:%02x\n", frame[32], frame[33], frame[34], frame[35], frame[36], frame[37]);
	printf("	IP address of target: %d.%d.%d.%d\n", frame[38], frame[39], frame[40], frame[41]);

	return ;
}
