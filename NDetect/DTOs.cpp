#include "pch.h"
#include "DTOs.h"


/*	
================================================================================
	Packet Function Implementaions
================================================================================
*/


Packet::Packet()
{
}

Packet::Packet(tm cTime, int bytes, ip_address sourceIP, u_short sourcePort, ip_address destIP, u_short destPort)
{
	// Set the Packet Capture Time
	this->capturedTime = cTime;
	
	// Set the packet byte count
	this->packetBytes = bytes;

	// Set the IP Address and Port both directions
	this->sourceIpAddr = sourceIP;
	this->sourcePort = sourcePort;
	this->destIpAddr = destIP;
	this->destPort = destPort;

	// Set the string versions of the IP
	this->destIpString = this->GetDestIP();
	this->sourceIpString = this->GetSourceIP();
	this->sourcePortString = this->GetSourcePort();
	this->destPortString = this->GetDestPort();
}


std::string Packet::GetSourceIP()
{
	char cAddr[1024];
	sprintf_s(cAddr, "%d.%d.%d.%d", this->sourceIpAddr.byte1, this->sourceIpAddr.byte2, this->sourceIpAddr.byte3, this->sourceIpAddr.byte4);
	return cAddr;
}

std::string Packet::GetSourcePort()
{
	char cPort[1024];
	sprintf_s(cPort, "%d", this->sourcePort);
	return cPort;
}

std::string Packet::GetDestIP()
{
	char cAddr[1024];
	sprintf_s(cAddr, "%d.%d.%d.%d", this->destIpAddr.byte1, this->destIpAddr.byte2, this->destIpAddr.byte3, this->destIpAddr.byte4);
	return cAddr;
}

std::string Packet::GetDestPort()
{
	char cPort[1024];
	sprintf_s(cPort, "%d", this->destPort);
	return cPort;
}


/*
================================================================================
	Connection Function Implementaions
================================================================================
*/

Connection::Connection() {


}