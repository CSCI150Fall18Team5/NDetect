#include "pch.h"
#include "CaptureEngine.h"

CaptureEngine::CaptureEngine()
{
}

CaptureEngine::~CaptureEngine()
{
}

void CaptureEngine::Init()
{


}

void CaptureEngine::SelectInterface()
{
	//  Find available devices
	if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
		return;
	}

	// Print the list 
	int i = 0;
	for (d = alldevs; d; d = d->next)
	{
		this->ifprint(d,++i);
	}

	printf("Enter the interface number (1-%d):", i);
	scanf_s("%d", &inum);

	if (inum < 1 || inum > i)
	{
		printf("\nInterface number out of range.\n");

		// Free the device list
		pcap_freealldevs(alldevs);
		return;
	}

	// Jump to the selected adapter
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);
	
	// Set the local Interface Name for use later
	interfaceName = d->name;
}

void CaptureEngine::Capture()
{	
	// Open the device 
	if ((fp = pcap_open(interfaceName.c_str(),
		100 /*snaplen*/,
		PCAP_OPENFLAG_PROMISCUOUS /*flags*/,
		20 /*read timeout*/,
		NULL /* remote authentication */,
		errbuf)
		) == NULL)
	{
		fprintf(stderr, "\nError opening adapter\n");
		return;
	}

	// Read the packets 
	while ((res = pcap_next_ex(fp, &header, &pkt_data)) >= 0)
	{
		if (res == 0)
			/* Timeout elapsed */
			continue;

		Sleep(500);

		// print pkt timestamp and pkt len
		printf("%ld:%ld (%ld)\n", header->ts.tv_sec, header->ts.tv_usec, header->len);

		// Loop through Packet data
		for (i = 1; (i < header->caplen + 1); i++)
		{
			/*
			printf("%.2x ", pkt_data[i - 1]);
			if ((i % LINE_LEN) == 0) printf("\n");
			*/
			
			// First print data in Hex
			printf("%.2X ", pkt_data[i - 1]);
			if ((i % LINE_LEN) == 0)
			{
				for (int j = i - LINE_LEN; j < i + LINE_LEN; j++)
				{
					// Print only ASCII Characters (For data veiwing purposes)
					if (pkt_data[j - 1] < 176 && pkt_data[j - 1] > 40)
					{
						printf("%c", pkt_data[j - 1]);
					}
					else
					{
						// Print a dot if out of ASCII range
						std::cout << ".";
					}
					
				}
				printf("\n");
			}
		}

		printf("\n\n");
	}

	if (res == -1)
	{
		fprintf(stderr, "Error reading the packets: %s\n", pcap_geterr(fp));
		return;
	}

	return;
}


/* Helper Functions


*/


/* Print all the available information on the given interface */
void CaptureEngine::ifprint(pcap_if_t *d, int i)
{
	pcap_addr_t *a;
	char ip6str[128];

	/* Name */
	printf("%i: %s\n", i , d->name);

	/* Description */
	if (d->description)
		printf("\tDescription: %s\n", d->description);

	/* Loopback Address*/
	printf("\tLoopback: %s\n", (d->flags & PCAP_IF_LOOPBACK) ? "yes" : "no");

	/* IP addresses */
	for (a = d->addresses; a; a = a->next) {
		printf("\tAddress Family: #%d\n", a->addr->sa_family);

		switch (a->addr->sa_family)
		{
		case AF_INET:
			printf("\tAddress Family Name: AF_INET\n");
			if (a->addr)
				printf("\tAddress: %s\n", iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
			if (a->netmask)
				printf("\tNetmask: %s\n", iptos(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr));
			if (a->broadaddr)
				printf("\tBroadcast Address: %s\n", iptos(((struct sockaddr_in *)a->broadaddr)->sin_addr.s_addr));
			if (a->dstaddr)
				printf("\tDestination Address: %s\n", iptos(((struct sockaddr_in *)a->dstaddr)->sin_addr.s_addr));
			break;

		default:
			printf("\tAddress Family Name: Unknown\n");
			break;
		}
	}
	printf("\n");
}

/* From tcptraceroute, convert a numeric IP address to a string */
#define IPTOSBUFFERS    12
char * CaptureEngine::iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3 * 4 + 3 + 1];
	static short which;
	u_char *p;

	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	_snprintf_s(output[which], sizeof(output[which]), sizeof(output[which]), "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}