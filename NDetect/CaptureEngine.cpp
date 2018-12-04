#include "pch.h"
#include "CaptureEngine.h"
#include "Filter.h"

CaptureEngine::CaptureEngine(ThreadManager * tm)
{
	// Set the pointer to the ThreadManager
	threadMan = tm;
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
	
	// Temp Address object
	pcap_addr_t *a = d->addresses;
	
	// Loop through as many times as 
	if (hostAddrHops > 1) {
		for (int i = 0; i <= hostAddrHops; i++)
		{
			if( a->next != nullptr )
				a = a->next;
		}

		HostIP = iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr);
	}

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

	/* Check the link layer. We support only Ethernet for simplicity. */
	if (pcap_datalink(pCapObj) != DLT_EN10MB)
	{
		fprintf(stderr, "\nThis program works only on Ethernet networks.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
	}

	// Set the Netmask
	if (d->addresses != NULL)
		/* Retrieve the mask of the first address of the interface */
		netmask = ((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* If the interface is without addresses we suppose to be in a C class network */
		netmask = 0xffffff;

	// compile the filter for TCP and UDP (tcp or udp, because we want both)
	if (pcap_compile(pCapObj, &fcode, "tcp or udp", 1, netmask) < 0)
	{
		fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
	}

	// set the filter
	if (pcap_setfilter(pCapObj, &fcode) < 0)
	{
		fprintf(stderr, "\nError setting the filter.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
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
	// Start the Timeout Thread
	threadMan->Threads[threadMan->ThreadCount++] = std::thread(&CaptureEngine::CheckTimeout, this);
	
	// Start the Display Thread
	threadMan->Threads[threadMan->ThreadCount++] = std::thread(&CaptureEngine::Display, this);

	// Read the packets 
	while ((res = pcap_next_ex(pCapObj, &header, &pkt_data)) >= 0)
	{	
		if (res == 0)
			/* Timeout elapsed */
			continue;
		
		// Kill this loop if the ThreadManager signals to stop all threads
		if (!threadMan->threadsContinue) {
			break;
		}

		// Extract TCP/IP Information and create our Packet object
		DecodePacket();

		// Wait
		// Sleep(25);
	}

	if (res == -1)
	{
		printf("Error reading the packets: %s\n", pcap_geterr(pCapObj));
		threadMan->EndThreads();
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

	// Get the time right now
	time_t rawTime;
	time(&rawTime);

	// Create tm with local time
	tm rightNow;
	// Update rightNow with rawTime
	localtime_s(&rightNow, &rawTime);

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
	Packet pkt = Packet(rightNow, header->len, ih->saddr, sport, ih->daddr, dport);
	
	// New packet decoded
	newPacketDecoded = true;

	// Lock down on all Packet altering code.
	std::unique_lock<std::mutex> uniqueLock(threadMan->muxPackets);	

	// Test for the limit on captured packets, and pop the back if we've reached it.
	while (capturedPackets.size() >= packetLimit) {
		capturedPackets.pop_back();
	}
	// Push this packet into the list.
	capturedPackets.push_front(pkt);
	// Update the packet count size
	packetCount = capturedPackets.size();

	uniqueLock.unlock();

	// Attempt to create a Connection or update existing Connection from this packet
	CreateOrUpdateConnection(pkt);

}

void CaptureEngine::Display()
{
	// Continue to display until threads are ending
	while (threadMan->threadsContinue) {

		// Lock down all other threads so we can display without anything changing
		std::unique_lock<std::mutex> lockConns(threadMan->muxConnections);
		std::unique_lock<std::mutex> lockPackets(threadMan->muxPackets);

		// Handle Console Display 
		if (consoleMode == LiveStream) {
			
			// Only display when new packets recieved
			if (newPacketDecoded) {

				// Display the time, Source/Dest IP addrs and ports.
				DisplayPacketHeader();

				// Displayed new packet, need to wait until new packet recieved
				newPacketDecoded = false;

				// Displays Packet data
				if (displayPacketData) {
					// Print out the inner packet data.
					DisplayPacketData();
					// Sleep a short while so that larger packets stay visible
					Sleep(livestreamSleepTime*header->caplen * 5);
					printf("\n\n");
				}
			}
		}
		else if (consoleMode == ConnectionsMade) {


			/*=====================================
			!!!	Clears the Console of all text !!!
			======================================= */
			system("cls");

			printf("| Packet Count: %i \t| Connection Count: %i\t| ", capturedPackets.size(), connections.size());

			// display target ip
			//ShowTargetIP();

			// Display the Connection Timeout
			printf(" Connection Timeout: %is \n\r", timeoutSeconds);

			printf("-----------------------------------------------------------------------------------------\n\r");
			printf(" Bytes \t| Packets\t|  Time \t| Source_IP:Port \t| Destination_IP:Port \n\r");
			printf("=========================================================================================\n\r");

			// Only show the 30 newest Connections		
			const int connectionLimit = 30;
			int counter = 0;
			// C++ magic right here.
			// Newer for loop using auto tree iterator.
			for (auto& c : connections)
			{
				if (counter >= connectionLimit) {
					break;
				}
				else {
					counter++;
				}

				// Get the second value of the map, that being the connection.
				Connection con = c.second;

				// Convert time to readable
				char pktTime[16];
				tm pktTM = con.GetLastPacketTime();
				strftime(pktTime, sizeof pktTime, "%H:%M:%S", &pktTM);

				if (myFilter->GetLocalTargetIP() == con.sourceIpString.c_str() || myFilter->GetDestTargetIP() == con.destIpString.c_str() || myFilter->GetLocalTargetPort() == con.sourcePortString.c_str() || myFilter->GetDestTargetPort() == con.destPortString.c_str()) {
					// printf("|> %s : %s --> %s : %s", con.sourceIpString, con.sourcePortString, con.destIpString, con.destPortString );
					printf(" %i \t| %i \t\t| %s \t| %s:%s \t| %s:%s \n", con.GetTotalBytes(), con.GetPacketCount(), pktTime, con.sourceIpString.c_str(), con.sourcePortString.c_str(), con.destIpString.c_str(), con.destPortString.c_str());
					// std::cout << "|> {" << i << "} " << con.sourceIpString << con.sourcePortString << con.destIpString << con.destPortString << "\n";
				}
				if (noFilter)
					printf(" %i \t| %i \t\t| %s \t| %s:%s \t| %s:%s \n", con.GetTotalBytes(), con.GetPacketCount(), pktTime, con.sourceIpString.c_str(), con.sourcePortString.c_str(), con.destIpString.c_str(), con.destPortString.c_str());
			}

			// Wait to update in this mode.
			Sleep(connectionsSleepTime);

		}

		lockConns.unlock();
		lockPackets.unlock();

		Sleep(10);
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
}

void CaptureEngine::DisplayPacketData()
{
	// Loop through Packet data
	for (i = 1; (i < header->caplen + 1); i++)
	{
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

std::string CaptureEngine::GetHostIP()
{
	return HostIP;
}

int CaptureEngine::GetPacketCount()
{
	return packetCount;
}

ConsoleMode CaptureEngine::GetConsoleMode()
{
	return consoleMode;
}

// Returns our running list
std::list<Packet> CaptureEngine::GetPacketList()
{
	return this->capturedPackets;
}

std::list<Packet> CaptureEngine::GetNLastPackets(int N)
{
	// Create temp lists
	std::list<Packet> cappedPackets = capturedPackets;
	std::list<Packet> tempList;

	// Go for min(N rounds, number of packets)
	int min = (cappedPackets.size() < N) ? cappedPackets.size() : N;
	for (int i = 0; i < min; i++)
	{
		// Add the front
		tempList.push_front(cappedPackets.front());
		// Pop front
		cappedPackets.pop_front();
	}

	return tempList;
	
}

std::list <Connection> CaptureEngine::GetConnections()
{
	// Make a temp list, fill it with all connections.
	std::list <Connection> connCopy;

	// Only process if threads should be running
	if (threadMan->threadsContinue) {
		// Lock the thread so we can safely access the List.
		std::unique_lock<std::mutex> uniqueLock(threadMan->muxConnections);
		for (auto& con : connections) {

			
			if (myFilter->GetLocalTargetIP() == con.second.sourceIpString.c_str() || myFilter->GetDestTargetIP() == con.second.destIpString.c_str() || myFilter->GetLocalTargetPort() == con.second.sourcePortString.c_str() || myFilter->GetDestTargetPort() == con.second.destPortString.c_str()) {
				connCopy.push_back(con.second);
			}
			if (noFilter)
				connCopy.push_back(con.second);


			
		}
		// Unlock, we made our copies
		uniqueLock.unlock();
	}
	return connCopy;
}

std::string CaptureEngine::ConstructKeyString(Packet pkt)
{
	// Used for creating the string key.
	char sKey[1000];

	// Format the key string
	std::string sIP = pkt.GetSourceIP();
	std::string sPort = pkt.GetSourcePort();

	std::string dIP = pkt.GetDestIP();
	std::string dPort = pkt.GetDestPort();

	sprintf_s(sKey, "%s:%s->%s:%s", sIP.c_str(), sPort.c_str(), dIP.c_str(), dPort.c_str());
	
	// Return it
	return sKey;
}

std::string CaptureEngine::ConstructKeyString(Connection con)
{
	// Used for creating the string key.
	char sKey[1000];

	// Format the key string
	std::string sIP = con.GetSourceIP();
	std::string sPort = con.GetSourcePort();

	std::string dIP = con.GetDestIP();
	std::string dPort = con.GetDestPort();

	sprintf_s(sKey, "%s:%s->%s:%s", sIP.c_str(), sPort.c_str(), dIP.c_str(), dPort.c_str());

	// Return it
	return sKey;
}

bool CaptureEngine::ConnectionExists(Packet pkt)
{
	std::string key = ConstructKeyString(pkt);
	if (connections.find(key) != connections.end()) {
		return true;
	}
	return false;
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
	// Lock the thread so we can safely modify the connection List.
	std::unique_lock<std::mutex> uniqueLock(threadMan->muxConnections);

	// Build the key string and check if the connection exists.
	std::string key = ConstructKeyString(pkt);
	if( ConnectionExists(pkt) ){
		Connection con = connections.at(key);
		// Check if the packet belongs in this connection
		if (con.PacketBelongs(pkt)) {
			// Add the Packet to this connection
			connections[key].AddPacket(pkt);
			// End the loop.
			return;
		}
	}
	else {
		// Create and assign the connection.
		Connection newConn(pkt);
		
		// Getting random exceptions here.
		try {
			// Maps insert like an array.
			connections[key] = newConn;
		} 
		catch (std::exception ex) {
			// Silently eat any exceptions.
		}
	}
	uniqueLock.unlock();

}

void CaptureEngine::CheckTimeout()
{
	while (threadMan->threadsContinue) {

		// Only process if connections present.
		if (connections.size() > 0) {
			// Used for the Packets
			int packetSeconds;

			// Get the time right now
			time_t rawTime;
			time(&rawTime);

			// Create tm with local time
			tm rightNow;
			// Update rightNow with rawTime
			localtime_s(&rightNow, &rawTime);

			// Convert min and seconds to seconds. (Minutes * 60) + Seconds 
			// Subtract Timeout value from seconds
			int timeoutSecondsAgo = (rightNow.tm_min * 60) + (rightNow.tm_sec - timeoutSeconds);

			std::string keysToRemove[1000];
			int keyCount = 0;

			// Unique Lock - Stops all other threads from operating while this thread modifies Connections
			std::unique_lock<std::mutex> uniqueLock(threadMan->muxConnections);

			// Loop through all connections
			for (auto& con : connections)
			{
				// Get the Packet time
				tm lastPacketTime = con.second.GetLastPacketTime();

				packetSeconds = (lastPacketTime.tm_min * 60) + lastPacketTime.tm_sec;

				// Over the Timeout Limit, erase the connection
				if (packetSeconds <= timeoutSecondsAgo) {
					// Store this key to remove after the loop
					keysToRemove[keyCount++] = con.first;
				}
			}

			// Loop through keys and remove the connections
			for (int i = 0; i < keyCount; i++)
			{
				// Remove the connection with the key
				connections.erase(keysToRemove[i]);
			}
			uniqueLock.unlock();
			
		}

		// Wait to try again for a fifth of the Timeout Time
		Sleep((DWORD)timeoutSeconds / 5);
	}

}

void CaptureEngine::SetTimeout(int seconds)
{
	if (seconds > 0 && seconds < 60) {
		timeoutSeconds = seconds;
	}
	else {
		timeoutSeconds = 10;
	}
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
		
		// Avoid printing if not a valid address family
		if (a->addr->sa_family == 23) { continue; }

		printf("\tAddress Family: #%d\n", a->addr->sa_family);

		switch (a->addr->sa_family)
		{
		case AF_INET:
			printf("\tAddress Family Name: AF_INET\n");
			if (a->addr)
			{
				printf("\tAddress: %s\n", iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
				hostAddrHops++;
			}
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
