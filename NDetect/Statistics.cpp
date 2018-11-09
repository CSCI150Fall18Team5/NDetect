#include "pch.h"
#include "Statistics.h"
#include "CaptureEngine.h"

CaptureEngine captureEngineStatistics;
Statistics::Statistics()
{
}


Statistics::~Statistics()
{
}

int Statistics::retNumOfPackets()
{
	int NumOfpacketsCaptured = captureEngineStatistics.GetPacketList().size();
	return NumOfpacketsCaptured;
}
