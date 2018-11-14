#include "pch.h"
#include "Statistics.h"
#include "CaptureEngine.h"

CaptureEngine captureEngineStatistics;
long int totalNetLength=0; 
struct pcap_pkthdr *header;

Statistics::Statistics()
{
}


Statistics::~Statistics()
{
}

//calculates the number of packets in the network
int Statistics::retNumOfPackets()
{
	int NumOfpacketsCaptured = captureEngineStatistics.GetPacketList().size();
	return NumOfpacketsCaptured;
}



//sums up the langth of all the packets in the network
long int Statistics::totalLengthOfPackets()
{
	bool stillCapturing = true;
	while (stillCapturing)
	{
		totalNetLength = totalNetLength + header->len;
	}
	return totalNetLength;
}
