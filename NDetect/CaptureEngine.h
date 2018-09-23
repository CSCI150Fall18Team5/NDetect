#pragma once

#include "pch.h"

#define LINE_LEN 16


class CaptureEngine
{
	// Private Vars Needed for WinPCap
	//  Interface holders
	pcap_if_t *alldevs, *d;
	// Holds the packet capture object
	pcap_t * pCapObj;
	u_int inum, i = 0;
	// Error buffer object
	char errbuf[PCAP_ERRBUF_SIZE];
	int res;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;

	// Class specific

	// Captures the interface name during SelectInterface()
	std::string interfaceName = "Test";
	// Slows the flow of packets on the console during DisplayPacketData()
	// packets stay on screen longer for longer packets of data.
	int sleepTime = .2;

public:
	CaptureEngine();
	~CaptureEngine();

	// Determines how the Engine should proceed
	void Init();
	// Displays the interfaces to choose from.
	void SelectInterface();
	// Taps into the capture engine
	void Capture(int mode);
	// Displays the packets being received from the capture stream.
	void DisplayPacketData();
	// Prints the interfaces
	void ifprint(pcap_if_t *d, int i);
	char * iptos(u_long in);
};

