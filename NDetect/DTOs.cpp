#include "pch.h"
#include "DTOs.h"


/*
================================================================================
	TCP/IP Function Implementaions
================================================================================
*/

bool ip_address::operator==(const ip_address & other)
{
	// Return true if all bytes match
	if (
		this->byte1 == other.byte1 &&
		this->byte2 == other.byte2 &&
		this->byte3 == other.byte3 &&
		this->byte4 == other.byte4
		) {
		return true;
	}
	// Else false
	return false;
}

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

Connection::Connection(Packet pkt)
{
	// Set the IP Address and Port both directions
	this->sourceIpAddr = pkt.sourceIpAddr;
	this->sourcePort = pkt.sourcePort;
	this->destIpAddr = pkt.destIpAddr;
	this->destPort = pkt.destPort;

	// Set the string versions of the IP
	this->destIpString = this->GetDestIP();
	this->sourceIpString = this->GetSourceIP();
	this->sourcePortString = this->GetSourcePort();
	this->destPortString = this->GetDestPort();

	// Connection Total bytes equals the incoming Packet bytes.
	this->totalBytes = pkt.packetBytes;
	this->packetBytes = pkt.packetBytes;

	// Set the packet Time
	this->lastPacketTime = pkt.capturedTime;

	// Increment Packet Count
	this->packetCount++;
}

bool Connection::PacketBelongs(Packet pkt)
{
	int mode = 0;
	if (mode == 0) {
		// Source IP / Port must match packet Source IP / Dest
		if (
			sourceIpAddr == pkt.sourceIpAddr &&
			sourcePort == pkt.sourcePort &&
			destIpAddr == pkt.destIpAddr &&
			destPort == pkt.destPort
			) return true;
	}
	else if (mode == 1) {
		// Match either direction
		if (
			// Source IP Matches either source or dest
			(sourceIpAddr == pkt.sourceIpAddr || sourceIpAddr == pkt.destIpAddr)
			&&
			// Source Port matches either source or Dest
			(sourcePort == pkt.sourcePort || sourcePort == pkt.destPort)
			&&
			// Dest IP Matches either source or dest
			(destIpAddr == pkt.destIpAddr || destIpAddr == pkt.sourceIpAddr)
			&&
			(destPort == pkt.destPort || destPort == pkt.sourcePort)
			) return true;
	}
	return false;
}

void Connection::AddPacket(Packet pkt)
{
	// Increment packet count
	packetCount++;
	// Add new packet bytes to connection
	totalBytes += pkt.packetBytes;
	// Update the last packet time
	lastPacketTime = pkt.capturedTime;

}

bool Connection::operator==(const Connection & other)
{
	// Return true if all IPs and Ports match
	if (this->sourceIpAddr == other.sourceIpAddr &&
		this->sourcePort == other.sourcePort &&
		this->destIpAddr == other.destIpAddr &&
		this->destPort == other.destPort) {
		return true;
	}
	return false;
}

int Connection::GetTotalBytes()
{
	return totalBytes;
}

int Connection::GetPacketCount()
{
	return packetCount;
}

tm Connection::GetLastPacketTime()
{
	return lastPacketTime;
}

bool Connection::isLocalHostConnection()
{
	return this->isLocalHost;
}


VisualObjectAttributes::VisualObjectAttributes()
{
}

// Rotate by adding double a to current angle.
void VisualObjectAttributes::IncRotate(double a) {
	this->angle = this->angle + a;
}
// Basic transformation and translation functions.
void VisualObjectAttributes::SetRotation(double a, double x, double y, double z) {
	this->angle = a;
	this->xAng = x;
	this->yAng = y;
	this->zAng = z;
}
void VisualObjectAttributes::SetShape(double r, double l, double t) {
	this->radius = r;
	this->slices = l;
	this->stacks = t;
}
void VisualObjectAttributes::SetRGBColor(double r, double g, double b) {
	this->Red = r;
	this->Blue = b;
	this->Green = g;
}
void VisualObjectAttributes::SetScaling(double x, double y, double z) {
	this->xSca = x;
	this->ySca = y;
	this->zSca = z;
}
void VisualObjectAttributes::SetPosition(double x, double y, double z) {
	this->xPos = x;
	this->yPos = y;
	this->zPos = z;
}

VisualConnection::VisualConnection()
{
	// Set initial values.
	radius = 0.15;
	SetRGBColor(0.0, 0.85, 0.15);

}

VisualConnection::VisualConnection(std::string ipAddr, std::string port)
{
	SourceIP = ipAddr;
	SourcePort = port;

	// Set initial values.
	radius = 0.15;
	SetRGBColor(0.0, 0.85, 0.15);

}

VisualConnection::VisualConnection(Connection con)
{
	// Set initial values.
	radius = 0.15;
	SetRGBColor(0.0, 0.85, 0.15);

	// Update Internal TCP/IP Vars
	// Update the Source
	SourceIP = con.GetSourceIP();
	SourcePort = con.GetSourcePort();

	this->packetCount = con.GetPacketCount();
	this->totalBytes = con.GetTotalBytes();
	this->isLocalHost = con.isLocalHostConnection();

}

VisualConnection::~VisualConnection()
{
}

