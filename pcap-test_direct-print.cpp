#include <pcap.h>
#include <stdio.h>
#include <cstring>
#include <stdint.h>
#include <netinet/in.h>

#define ETHER_ADDR_LEN 6
#define IP_ADDR_LEN 4
#define PAYLOAD_LEN 8
#define ETHER_TYPE_LEN 2


void usage() {
    printf("syntax: pcap-test <interface>\n");
    printf("sample: pcap-test wlan0\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        usage();
        return -1;
    }
    int i = 0;
    int index = 0;
    char* dev = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "pcap_open_live(%s) return nullptr - %s\n", dev, errbuf);
        return -1;
    }

    while (true) {
        struct pcap_pkthdr* header;
        const u_char* packet;

        int res = pcap_next_ex(handle, &header, &packet);
        if (res == 0) continue;
        if (res == -1 || res == -2) {
            printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(handle));
            break;
        }

	printf("\nEthernet Header\n");
	printf("\nDEST MAC : ");
	for(i=0; i<=5; i++)
            printf("%02x ", packet[i]);
	printf("\nSRC MAC : ");
	for(i=6; i<=11; i++)
            printf("%02x ", packet[i]);
	printf("\nETH TYPE : ");
	for(i=12; i<=13; i++)
            printf("%02x ", packet[i]);
        
    	printf("\n\nIPv4 Header\n");	
	printf("\nIP Version and Header length : ");
	printf("%02x ", packet[14]);
	printf("\nSRC IP : ");
	for(i=26; i<=29; i++)
            printf("%d.", packet[i]);
	printf("\nDEST IP : ");
	for(i=30; i<=33; i++)
            printf("%d.", packet[i]);

	printf("\n\nTCP Header\n");
	printf("\nSRC PORT : ");
	for(i=34; i<=35; i++)
            printf("%02x ", packet[i]);
	printf("\nDEST PORT : ");
	for(i=36; i<=37; i++)
            printf("%02x ", packet[i]);
	printf("\nTCP PAYLOAD : ");
	for(i=54; i<=69; i++)
            printf("%02x ", packet[i]);
            
        printf("\n%u bytes captured\n", header->caplen);
    }

    pcap_close(handle);
}