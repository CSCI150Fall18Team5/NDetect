#include "pch.h"
#include "CaptureEngine.h"
#include <string>
#include <iostream>


CaptureEngine::CaptureEngine()
{
}

CaptureEngine::~CaptureEngine()
{
}

// Private variable setter functions
void CaptureEngine::SetCaptureMode(int mode) 
{
	this->captureMode = mode;
}

void CaptureEngine::SetConsoleMode(ConsoleMode cm)
{
	this->consoleMode = cm;
}

void CaptureEngine::SetLiveStreamDisplay(PacketDisplay pcktdisp)
{
	this->displayPacketData = pcktdisp;
}


void CaptureEngine::SelectInterface()
{
	//  Find available devices
	if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		printf("Error in pcap_findalldevs_ex: %s\n", errbuf);
		return;
	}

	// Print the list of devices
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

	// Jump to the selected adapter
	for (d = alldevs, i = 0; i < (int)inum - 1; d = d->next, i++);
	
	// Set the local Interface Name for use later
	interfaceName = d->name;
}

void CaptureEngine::Capture()
{	
	// Open the device 
	if ((pCapObj = pcap_open(interfaceName.c_str(),
		100 /*snaplen - integer which defines the maximum number of bytes to be captured by pcap*/,
		0 /*flags*/,
		20 /*read timeout*/,
		NULL /* remote authentication */,
		errbuf)
		) == NULL)
	{
		printf("\nError opening adapter\n");
		return;
	}

	// Free the devices, since we choose ours already.
	pcap_freealldevs(alldevs);


	// Now that the pCapObj is created, we can just tap into the capture stream.
	this->CaptureLoop();

	return;
}

// Loop function that uses packets from the PCAP interface to display packet data.
void CaptureEngine::CaptureLoop()
{
	// Read the packets 
	while ((res = pcap_next_ex(pCapObj, &header, &pkt_data)) >= 0 && continueCapturing)
	{
		if (res == 0)
			/* Timeout elapsed */
			continue;

	
		

		// Extract TCP/IP Information and create our Packet object
		DecodePacket();


		// Display changes to the console
		Display();
		
		/* ZS
		May experiment with threading the display function... 
		std::thread displayThread(&CaptureEngine::Display, this);
		*/
		
	}

	// If we stop the output by triggering CTRL+C
	// Just sleep until the program quits
	while (!continueCapturing) {
		Sleep(1000);
	}

	if (res == -1)
	{
		printf("Error reading the packets: %s\n", pcap_geterr(pCapObj));
		return;
	}
}

// Pulls out the relevant information from the raw packet bytes.
// Creates the Packets objects that we will use throughout the rest of the program
void CaptureEngine::DecodePacket()
{
	// convert the timestamp to readable format 
	local_tv_sec = header->ts.tv_sec;
	localtime_s(&ltime, &local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", &ltime);

	// retrieve the position of the ip header 
	ih = (ip_header *)(pkt_data + 14); //length of ethernet header

	// retireve the position of the udp header 
	ip_len = (ih->ver_ihl & 0xf) * 4;
	uh = (udp_header *)((u_char*)ih + ip_len);

	// convert from network byte order to host byte order 
	sport = ntohs(uh->sport);
	dport = ntohs(uh->dport);

	// --------------- END Copywritten Code ---------------

	// Create our Packet object, and push into our list
	Packet pkt = Packet(ltime, header->len, ih->saddr, sport, ih->daddr, dport);
	
	// Test for the limit on captured packets, and pop the back if we've reached it.
	while (capturedPackets.size() >= packetLimit) {
		capturedPackets.pop_back();
	}
	// Push this packet into the list.
	capturedPackets.push_front(pkt);

	// Attempt to create a Connection or update existing Connection from this packet
	CreateOrUpdateConnection(pkt);

}

void CaptureEngine::Display()
{
	// Handle Console Display 
	if (consoleMode == LiveStream) {

		// Display the time, Source/Dest IP addrs and ports.
		DisplayPacketHeader();

		// Displays Packet data
		if (displayPacketData) {
			// Print out the inner packet data.
			DisplayPacketData();
			// Sleep a short while so that larger packets stay visible
			Sleep(sleepTime*header->caplen * 5);
			printf("\n\n");
		}
	}
	else if (consoleMode == ConnectionsMade) {

		
		/*=====================================
		!!!	Clears the Console of all text !!!
		======================================= */
		system("cls");


		// printf("Current Connections: \n\r");
		printf("| Packet Count: %i \t| Connection Count: %i\t| ", capturedPackets.size(), connectionCount);
		
		// display target ip
		ShowTargetIP();

		printf("-----------------------------------------------------------------------------------------\n\r");
		printf(" Bytes \t| Packets\t|  Time \t| Source_IP:Port \t| Destination_IP:Port \n\r");
		printf("=========================================================================================\n\r");
		// Only show the 25 newest Connections
		int max = (connectionCount - 30 >= 0) ? connectionCount - 30 : 0;
		for (int i = connectionCount - 1; i > max; i--)
		{
			Connection con = connections[i];

			// Convert time to readable
			char pktTime[16];
			tm pktTM = con.GetLastPacketTime();
			strftime(pktTime, sizeof pktTime, "%H:%M:%S", &pktTM);


			if (GetTargetIP() == con.sourceIpString.c_str() || GetTargetIP() =="") {
				// printf("|> %s : %s --> %s : %s", con.sourceIpString, con.sourcePortString, con.destIpString, con.destPortString );
				printf(" %i \t| %i \t\t| %s \t| %s:%s \t| %s:%s \n", con.GetTotalBytes(), con.GetPacketCount(), pktTime, con.sourceIpString.c_str(), con.sourcePortString.c_str(), con.destIpString.c_str(), con.destPortString.c_str());
				// std::cout << "|> {" << i << "} " << con.sourceIpString << con.sourcePortString << con.destIpString << con.destPortString << "\n";
			}


		}
		Sleep(30);
	}
}


void CaptureEngine::DisplayPacketHeader() {

	// print timestamp, packet number, and length of the packet 
	printf("|> {%s.%.6d}\t| Packet # %i\t| length:%d\t| ", timestr,  header->ts.tv_usec, capturedPackets.size(), header->len);

	// print ip addresses and udp ports 
	printf("%d.%d.%d.%d:%d --> %d.%d.%d.%d:%d\n",
		ih->saddr.byte1,
		ih->saddr.byte2,
		ih->saddr.byte3,
		ih->saddr.byte4,
		sport,
		ih->daddr.byte1,
		ih->daddr.byte2,
		ih->daddr.byte3,
		ih->daddr.byte4,
		dport);

	// print pkt timestamp and pkt len
	// printf("%ld:%ld (%ld)\n", header->ts.tv_sec, header->ts.tv_usec, header->len);
	
}

void CaptureEngine::DisplayPacketData()
{
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
			// Then print the next LINE_LEN in ASCII
			for (int j = i - LINE_LEN; j < (int)i + LINE_LEN; j++)
			{
				// Print only ASCII Characters (For data veiwing purposes)
				if (pkt_data[j - 1] < 128 && pkt_data[j - 1] > 32)
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

}

// Returns our running list
std::list<Packet> CaptureEngine::GetPacketList()
{
	return this->capturedPackets;
}

void CaptureEngine::SetContinueCapturing(bool val)
{
	this->continueCapturing = val;
}

void CaptureEngine::SetPacketLimit(int max)
{
	// Must be between 0 and maximum integer value
	if (max >= 0 and max <= INT32_MAX)
		packetLimit = max;
	else
		packetLimit = 20000;
}

void CaptureEngine::CreateOrUpdateConnection(Packet pkt)
{
	// Check each one
	for (size_t i = 0; i < connectionCount; i++)
	{
		// Test Connection obtained from the front of the list
		Connection con = connections[i];

		// Check if the packet belongs in this connection
		if (con.PacketBelongs(pkt)) {
			// Add the Packet to this connection
			connections[i].AddPacket(pkt);
			// End the loop.
			return;
		}
	}
	
	// No matching Connections found, create a new one.
	Connection newConn(pkt);
	connections[connectionCount++] = newConn;

}


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


// Set target IP
void CaptureEngine::SetTargetIP(std::string IP)
{
	targetIP = IP;
}

// Get target IP
std::string CaptureEngine::GetTargetIP()
{
	return targetIP;
}

// Method to show Target IP
void CaptureEngine::ShowTargetIP()
{
	std::cout << "Target IP: " << targetIP << " \t|\n\r";
}