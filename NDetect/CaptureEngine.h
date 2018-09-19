#pragma once

#include "pch.h"

#define LINE_LEN 16

class CaptureEngine
{
	// Private Vars
	// Needed for WinPCap
	pcap_if_t *alldevs, *d;
	pcap_t *fp;
	u_int inum, i = 0;
	char errbuf[PCAP_ERRBUF_SIZE];
	char * infName;
	int res;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;

	// Class specific
	std::string interfaceName = "Test";
	std::string Test;

public:
	CaptureEngine();
	~CaptureEngine();

	// Determines how the Engine should proceed
	void Init();
	void SelectInterface();
	void Capture();

	void ifprint(pcap_if_t *d, int i);
	char * iptos(u_long in);
};

