#include "../includes/ft_malcolm.h"

void free_subnet(char **s1, char **s2){
	int i;

	for (i = 0; s1[i] != NULL; i++)
		free(s1[i]);
	free(s1);
	for (i = 0; s2[i] != NULL; i++)
		free(s2[i]);
	free(s2);
	return ;
}

void free_av(int i, char **av){
	if (i == 1)
		free(av[1]);
	else if (i == 2)
		free(av[3]);
	else{
		free(av[1]);
		free(av[3]);
	}
	return ;
}

void free_addresses(char **sha, char **tha, char **spa, char **tpa){
	int i;

	for (i = 0; sha[i] != NULL; i++)
		free(sha[i]);
	free(sha);
	for (i = 0; tha[i] != NULL; i++)
		free(tha[i]);
	free(tha);
	for (i = 0; spa[i] != NULL; i++)
		free(spa[i]);
	free(spa);
	for (i = 0; tpa[i] != NULL; i++)
		free(tpa[i]);
	free(tpa);
	return ;
}

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
	frame[21] == 1 ? printf("	MAC address of target: ff:ff:ff:ff:ff:ff\n") : printf("	MAC address of target: %02x:%02x:%02x:%02x:%02x:%02x\n", frame[32], frame[33], frame[34], frame[35], frame[36], frame[37]);
	printf("	IP address of target: %d.%d.%d.%d\n", frame[38], frame[39], frame[40], frame[41]);

	return ;
}
