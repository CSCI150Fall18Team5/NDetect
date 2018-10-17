#pragma once
#include "pch.h"

/*
	DTOs - (Data Transfer Objects)
	Use this space for structs/classes that 
	don't have many functions, and mostly 
	carry data around the program.
*/

enum PacketDisplay
{
	HeaderOnly, RawData
};

enum ConsoleMode
{
	Statistics, LiveStream, Combo
};

/* 4 bytes IP address */
struct ip_address {
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
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
} ;



// Keeping as a class instead of struct so we can destroy it properly.
class Packet
{
	// String Representation
	std::string sourceIpString;
	std::string destIpString;
	
	// Source IP
	ip_address sourceIpAddr;
	// Source Port
	u_short sourcePort;

	// Destination IP 
	ip_address destIpAddr;
	// Destination Port
	u_short destPort;

public:

	// Blank
	Packet();
	// Fills out TCP/IP info
	Packet(ip_address sourceIP, u_short sourcePort, ip_address destIP, u_short destPort);

	// Figure out how to properly destory..
	~Packet();

	/* Sets the packet data
	void SetSource(u_char sByte1, u_char sByte2, u_char sByte3, u_char sByte4, u_short sPort);
	void SetSource(ip_address sourceIP, u_short sPort);
	void SetDestination(u_char dByte1, u_char dByte2, u_char dByte3, u_char dByte4, u_short dPort);
	void SetDestination(ip_address destIp, u_short dPort);
	*/

	std::string GetSourceIP();
	std::string GetSourcePort();
	std::string GetDestIP();
	std::string GetDestPort();

};


//  Inherits from Packet to automatically have TCP/IP information.
class Connection : private Packet {

	// Store the packets of this connection there
	std::stack<Packet> packets;

	Connection();
	Connection(Packet pkt);

	void AddPacket(Packet newPkt);

};
