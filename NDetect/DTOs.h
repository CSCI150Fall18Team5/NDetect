#pragma once
#include "pch.h"

/*
	DTOs - (Data Transfer Objects)
	Use this space for structs/classes that 
	don't have many functions, and mostly 
	carry data around the program.
*/


/*
	Enums used in CaptureEngine
*/
enum PacketDisplay
{
	HeaderOnly, RawData
};
enum ConsoleMode
{
	Statistics, LiveStream, Combo, ConnectionsMade
};


// This enum refers to the programThreads array back in NDetect.cpp
// Enum values start at 0
// To create new threads, create a new entry in this enum
// Then, you can refence your thread like so:
// Ex. programThreads[NewThreadEnum].join()
enum ThreadID {
	CaptureLoop, ThreadedPrint
};

/* 4 bytes IP address */
class ip_address {
public:
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;

	// Create the equal operator for easier comparison
	bool operator==(const ip_address& other);

};

/* IPv4 header */
struct ip_header {
	u_char  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
	u_char  tos;            // Type of service 
	u_short tlen;           // Total length 
	u_short identification; // Identification
	u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
	u_char  ttl;            // Time to live
	u_char  proto;          // Protocol
	u_short crc;            // Header checksum
	ip_address  saddr;      // Source address
	ip_address  daddr;      // Destination address
	u_int   op_pad;         // Option + Padding
};

/* UDP header*/
struct udp_header {
	u_short sport;          // Source port
	u_short dport;          // Destination port
	u_short len;            // Datagram length
	u_short crc;            // Checksum
};

/* TCP header */
struct tcp_header
{
	unsigned short sPort; // source port
	unsigned short dPort; // destination port
};

// Keeping as a class instead of struct so we can destroy it properly.
class Packet
{
public:
	// Source IP
	ip_address sourceIpAddr;
	// Source Port
	u_short sourcePort;

	// Destination IP 
	ip_address destIpAddr;
	// Destination Port
	u_short destPort;

	// String Representations
	std::string sourceIpString;
	std::string destIpString;
	std::string sourcePortString;
	std::string destPortString;

	// Packet interception time
	struct tm capturedTime;

	// Packet Byte Length
	int packetBytes;

	// Blank
	Packet();
	// Fills out TCP/IP info
	Packet(tm cTime, int bytes, ip_address sourceIP, u_short sourcePort, ip_address destIP, u_short destPort);

	// String versions
	std::string GetSourceIP();
	std::string GetSourcePort();
	std::string GetDestIP();
	std::string GetDestPort();

};


// Inherits from Packet to automatically have TCP/IP information.
// Used to match new packets coming into the system
class Connection : public Packet {

	// Number of packets that match this connection
	int packetCount = 0;

	// Combined length of packets for this connection
	int totalBytes = 0;

public:

	Connection();
	// Contructor to set the connection endpoints (IP:port -> IP:port)
	Connection(Packet pkt);

	bool PacketBelongs(Packet pkt);
	void AddPacket(Packet pkt);

	int GetTotalBytes();
	int GetPacketCount();
};
