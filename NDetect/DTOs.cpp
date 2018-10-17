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

Packet::Packet(ip_address sourceIP, u_short sourcePort, ip_address destIP, u_short destPort)
{
	this->sourceIpAddr = sourceIP;
	this->sourcePort = sourcePort;
	this->destIpAddr = destIP;
	this->destPort = destPort;
}


Packet::~Packet()
{
}

/*

	============ Might not need these functions, commenting out for now. ============

void Packet::SetSource(u_char sByte1, u_char sByte2, u_char sByte3, u_char sByte4, u_short s_port)
{
	this->sourceIpAddr.byte1 = sByte1;
	this->sourceIpAddr.byte2 = sByte2;
	this->sourceIpAddr.byte3 = sByte3;
	this->sourceIpAddr.byte4 = sByte4;
	this->sourcePort = s_port;
}

void Packet::SetSource(ip_address sourceIP, u_short sPort)
{
	this->destIpAddr = sourceIP;
	this->sourcePort = sPort;
}


void Packet::SetDestination(u_char dByte1, u_char dByte2, u_char dByte3, u_char dByte4, u_short d_port)
{
	this->destIpAddr.byte1 = dByte1;
	this->destIpAddr.byte2 = dByte2;
	this->destIpAddr.byte3 = dByte3;
	this->destIpAddr.byte4 = dByte4;
	this->destPort = d_port;

}

void Packet::SetDestination(ip_address destIp, u_short dPort)
{
	this->destIpAddr = destIp;
	this->destPort = dPort;
}

*/

// TODO Properly Implement these

std::string Packet::GetSourceIP()
{
	return std::string();
}

std::string Packet::GetSourcePort()
{
	return std::string();
}

std::string Packet::GetDestIP()
{
	return std::string();
}

std::string Packet::GetDestPort()
{
	return std::string();
}


/*
================================================================================
	Connection Function Implementaions
================================================================================
*/

Connection::Connection() {


}