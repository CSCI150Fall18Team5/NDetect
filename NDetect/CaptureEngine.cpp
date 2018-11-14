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
void CaptureEngine::Menu() {
	while (TRUE) {
		printf("Please choose a menu option: (1) Select Packet Limit , (2) Connection timeout (sec), (3) Network Interface , (4) Console Output, (5) Start Capture");

		scanf_s("%d", &m);
		int menu = (int)m;
		if (menu < 1 || menu > i) {
			printf("Input is out of range");
			return;
		}
		switch (menu) {
		case 1:
			scanf_s("%d", &limit);
			int pac_limit = (int)limit;
			Capture();
		case 2:
			scanf_s("%d", &time);
			int conn_time = (int)time;
			Capture();
		case 3:
			SelectInterface();
		case 4:
			Display_Mode();
		case 5:
			Capture();
		default:
			printf("Input is invalie, please choose again");
			break;
		}
	}

}

void CaptureEngine::Display_Mode() {
	printf("Choose type of Mode: (1)Live Stream, (2) Statistic, (3) Both Stats N Live, (4)Connections");
	scanf_s("%d", &input);
	int temp = (int)input;
	if (temp < 1 || temp > i) {
		printf("Input is out of range");
		return;
		DisplayPacketData(input);

	}
}
void CaptureEngine::SelectInterface()
{
	//  Find available devices
	if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
		return;
	}
	//Display_Mode();
	// Print the list
	int i = 0;
	for(d = alldevs; d; d = d->next)
	{
		/* Print all the available information on the given interface */
		this->ifprint(d,++i);
	}

	printf("Enter the interface number (1-%d):", i);
	scanf_s("%d", &inum);
	
	if ((int)inum < 1 || (int)inum > i)
	{
		printf("\nInterface number out of range.\n");

		// Free the device list
		pcap_freealldevs(alldevs);
		return;
	}
	Display_Mode();

	// Jump to the selected adapter
	for (d = alldevs, i = 0; i < (int)inum - 1; d = d->next, i++);
	
	// Set the local Interface Name for use later
	interfaceName = d->name;
}

void CaptureEngine::Capture(int mode = 0)
{
			printf("Value of i: ", i);
			if ((pCapObj = pcap_open(interfaceName.c_str(),
				100 /*snaplen*/,
				mode /*flags*/,
				20 /*read timeout*/,
				NULL /* remote authentication */,
				errbuf)
				) == NULL)
			{
				fprintf(stderr, "\nError opening adapter\n");
				return;
			}
	
		// Now that the pCapObj is created, we can just tap into the capture stream.
		this->DisplayPacketData(input);
	
	
	return;
}

// Loop function that uses packets from the PCAP interface to display packet data.
void CaptureEngine::DisplayPacketData(int input)
{
	int range = 0;
	//this->Display_Mode();
	switch (input)
	{
	case 1:
	 range = 50;
	case 2:
		//Statistic_Display()
	case 3:
		//Combined_Display()
	case 4:
		//string Connection 
	default: printf("Input is invalid");

		break;
	}
	int defaultpacket_size = 50;
	for (int i = 0; i <= defaultpacket_size; i++) {
		// Open the device 
		if (i == defaultpacket_size) { break; }

	// Read the packets 
	while ((res = pcap_next_ex(pCapObj, &header, &pkt_data)) >= 0)
	{
		if (res == 0)
			/* Timeout elapsed */
			continue;

		// Removing sleep for now... ZS
		// Let the packets show up as quickly as we are capturing them
		// Sleep(sleepTime*header->caplen);

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
				for (int j = i - LINE_LEN; j < (int)i + LINE_LEN; j++)
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
		fprintf(stderr, "Error reading the packets: %s\n", pcap_geterr(pCapObj));
		return;
	}
}


//void CaptureEngine::LiveStream() {
//}
/* Helper Functions


*/


/* Print all the available information on the given interface */
void CaptureEngine::ifprint(pcap_if_t *d, int i)
{
	pcap_addr_t *a;
	// char ip6str[128];

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
