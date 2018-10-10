#pragma once

/*
	DTOs - (Data Transfer Objects)
	Use this space for structs/classes that 
	don't have many functions, and mostly 
	carry data around the program.
*/


// Keeping as a class instead of struct so we can destroy it properly.
class Packet
{
	// String Representation
	std::string source_ip_addr;
	std::string dest_ip_addr;
	
	// Source IP bytes
	u_char source_byte1;
	u_char source_byte2;
	u_char source_byte3;
	u_char source_byte4;
	// Source Port
	u_short source_port;

	// Destination IP Bytes
	u_char dest_byte1;
	u_char dest_byte2;
	u_char dest_byte3;
	u_char dest_byte4;
	// Destination Port
	u_short dest_port;

public:
	
	// This constructor looks bad, but in use it's nicer..
	Packet(u_char source_byte1, u_char source_byte2, u_char source_byte3, u_char source_byte4, u_short source_port, u_char dest_byte1, u_char dest_byte2, u_char dest_byte3, u_char dest_byte4, u_short dest_port);
	
	// Blank
	Packet();

	// Figure out how to properly destory..
	~Packet();

	// Sets the packet data
	void SetSource(u_char s_byte1, u_char s_byte2, u_char s_byte3, u_char s_byte4, u_short s_port);
	void SetDestination(u_char d_byte1, u_char d_byte2, u_char d_byte3, u_char d_byte4, u_short d_port);


};

enum ConsoleMode
{
	Statistics, LiveStream, Combo
};

/* 4 bytes IP address */
typedef struct ip_address {
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header {
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
}ip_header;

/* UDP header*/
typedef struct udp_header {
	u_short sport;          // Source port
	u_short dport;          // Destination port
	u_short len;            // Datagram length
	u_short crc;            // Checksum
}udp_header;
