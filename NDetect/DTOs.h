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


// Used for Thread array index
// Example Thread Creation:
// threadMan->Threads[ThreadNames::Timeout] = std::thread(&CaptureEngine::CheckTimeout, this);
enum ThreadNames
{
	CaptureLoop, Timeout, UpdateConnections
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

	// Used for checking how long ago this connection was used
	struct tm lastPacketTime;

	bool isLocalHost = false;

public:

	Connection();
	// Contructor to set the connection endpoints (IP:port -> IP:port)
	Connection(Packet pkt);
	
	// Packet related functions
	bool PacketBelongs(Packet pkt);
	void AddPacket(Packet pkt);

	// Operators
	bool operator==(const Connection& other);

	// Getter functions
	int GetTotalBytes();
	int GetPacketCount();
	struct tm GetLastPacketTime();
	bool isLocalHostConnection();
};



// Visual Object Attributes
// Declare a structure for our 3D object
class VisualObjectAttributes {
public:
	// Attributes
	double Red = 0, Green = 0, Blue = 0;
	double xPos = 0, yPos = 0, zPos = 0;
	double xRot = 0, yRot = 0, zRot = 0;
	double xSca = 1.0, ySca = 1.0, zSca = 1.0;
	double radius = 0, slices = 0, stacks = 0;
	double angle = 0, xAng = 0, yAng = 1, zAng = 0;

	// Empty Constructor
	VisualObjectAttributes();
	
	// Rotate by adding double a to current angle.
	void IncRotate(double a);

	// Basic transformation and translation functions.
	void SetRotation(double a, double x, double y, double z);
	void SetShape(double r = 0, double l = 0, double t = 0);
	void SetRGBColor(double r = 0, double g = 0, double b = 0);
	void SetScaling(double x = 0, double y = 0, double z = 0);
	void SetPosition(double x = 0, double y = 0, double z = 0);
};


// VisualConnection
// Encapsulates a connection, and gives it properties usable in the GraphicsEngine

class VisualConnection : public VisualObjectAttributes {

public:
	// Static IP Address and Port
	std::string SourceIP;
	std::string SourcePort;

	std::list<std::string> DestIPs;

	// Set when this Connection is from the local host
	bool isLocalHost = false;
	// Number of packets that match this connection
	int packetCount = 0;
	// Combined length of packets for this connection
	int totalBytes = 0;
	
	// Contructors
	VisualConnection();
	VisualConnection(std::string ipAddr, std::string port);
	VisualConnection(Connection con);
	~VisualConnection();

	// VisualConnection& operator=(const Connection con);
};

