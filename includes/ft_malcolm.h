#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# define _GNU_SOURCE
# define BUFLEN 512	//Max length of buffer
# define PORT 8888	//The port on which to listen for incoming data
# define BUF_SIZE 42
# define DEVICE "eth0"
# define ETH_P_NULL 0x0
# define ETH_MAC_LEN ETH_ALEN
# define ETH_ARP 0x0806
#include <linux/if_packet.h>
#include <linux/if_ether.h>
//#include <linux/if_arp.h>

# include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <signal.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <ifaddrs.h>
# include <net/ethernet.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
//#include <net/if_arp.h>
/*  Ethernet frame header length */
#define ETHER_HEADER_LEN sizeof(struct ether_header)
 /*  The length of the entire arp structure */
#define ETHER_ARP_LEN sizeof(struct ether_arp)
 /*  Ethernet + entire arp structure length */
#define ETHER_ARP_PACKET_LEN (ETHER_HEADER_LEN+ETHER_ARP_LEN)
 /*  IP address length */
#define IP_ADDR_LEN 4
#define MAC_ADDR_LEN 6
typedef struct _arp_hdr arp_hdr;
struct _arp_hdr {
  uint16_t htype;
  uint16_t ptype;
  uint8_t hlen;
  uint8_t plen;
  uint16_t opcode;
  uint8_t sender_mac[6];
  uint8_t sender_ip[4];
  uint8_t target_mac[6];
  uint8_t target_ip[4];
};

/*struct __attribute__((packed)) arp_header
{
    unsigned short arp_hd;
    unsigned short arp_pr;
    unsigned char arp_hdl;
    unsigned char arp_prl;
    unsigned short arp_op;
    unsigned char arp_sha[6];
    unsigned char arp_spa[4];
    unsigned char arp_dha[6];
    unsigned char arp_dpa[4];
};*/

int	main(int ac, char **av);
int	arp_reception();
int arp_reply(char **av);
void fill_arphdr(arp_hdr *arphdr, char **av);
uint8_t hex_simple(char *s);
void fill_device(struct sockaddr_ll *device, uint8_t addr[]);
#endif
