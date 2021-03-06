#pragma once

#include "pch.h"
#include "DTOs.h"
#include "Filter.h"
#include "ThreadManager.h"
#include <string>
#include <iostream>


/* Portions of the code contained in this file, and CaptureEngine.cpp require this copyright.

 * Copyright (c) 1999 - 2005 NetGroup, Politecnico di Torino (Italy)
 * Copyright (c) 2005 - 2006 CACE Technologies, Davis (California)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Politecnico di Torino, CACE Technologies
 * nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


 /*
	 TODO:
		 Write functions to sort Packets into Connections
 */

#define LINE_LEN 16

class CaptureEngine
{
	// Private Vars Needed for WinPCap
	//  Interface holders
	pcap_if_t *alldevs, *d;
	// Holds the packet capture object
	pcap_t * pCapObj;
	u_int inum, i = 0;
	// Error buffer object
	char errbuf[PCAP_ERRBUF_SIZE];
	int res;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	// Used for Filtering
	u_int netmask;
	struct bpf_program fcode;

	// Used in Decoding the packet
	struct tm ltime;
	char timestr[16];
	ip_header *ih;
	udp_header *uh;
	u_int ip_len;
	u_short sport, dport;
	time_t local_tv_sec;

	// Configuration Vars

	/* Determines capture mode. 
	1 = Defines if the adapter has to go in promiscuous mode.
	2 = Defines if the data trasfer(in case of a remote capture) has to be done with UDP protocol.
	4 = Defines if the remote probe will capture its own generated traffic.
	8 = Defines if the local adapter will capture its own generated traffic.
	16 = This flag configures the adapter for maximum responsiveness.
	// Set SetCaptureMode() for more details */
	int captureMode = 8;

	// Set the console visualization mode
	ConsoleMode consoleMode = ConnectionsMade;

	// Switch to show packet data in Live Stream Mode.
	PacketDisplay displayPacketData = HeaderOnly;

	// Captures the interface name during SelectInterface()
	std::string interfaceName = "";

	// Host IP Address
	std::string HostIP;
	int hostAddrHops = 0;

	// Sleep times, to force the program to take it's time
	double captureSleepTime = 0.2, livestreamSleepTime = 0.5, connectionsSleepTime = 200;


	// Manages running threads and holds between them.
	ThreadManager * threadMan;

	// Timeout value in seconds
	int timeoutSeconds = 5;

	// List of Packets captured
	std::list<Packet> capturedPackets;
	int packetLimit = 20000;
	int packetCount = 0;
	bool newPacketDecoded = false;

	// Connection map
	/*
		The hash for this map will be {Source_IP:S_Port->Destination_IP:D_Port} as a string
	*/
	std::map <std::string, Connection> connections;

	// Prints the interfaces
	void ifprint(pcap_if_t *d, int i);
	// IP to string conversion
	char * iptos(u_long in);

public:
	CaptureEngine(ThreadManager * tm);
	~CaptureEngine();

	// Displays the interfaces to choose from.
	void SelectInterface();

	// Sets the mode of Capture
	// Mode: 1 = Promiscuous, 0 = Normal
	// Normal mode means only packets to and from the host machine are captured.
	// Promiscuous means that all packets, even those not destined for the
	// host computer are captured. This provides access to all packets that the
	// network card can see. 
	void SetCaptureMode(int);

	// Sets the output of the console.
	void SetConsoleMode(ConsoleMode cm);

	// Set the way live stream is displayed
	void SetLiveStreamDisplay(PacketDisplay);

	// Taps into the WinPCap library to start capturing packets
	void Capture();

	// Uses a while loop to capture new packets.
	void CaptureLoop();

	// Extracts Useful packet info like the IP addrs and Ports
	void DecodePacket();

	// Handles showing changes in the console.
	void Display();

	// Displays the packet in the live stream format
	void DisplayPacketHeader();

	// Shows the data inside the packet
	void DisplayPacketData();

	// Simple Getters
	std::string GetHostIP();

	int GetPacketCount();

	ConsoleMode GetConsoleMode();

	// Getter for Packet List
	std::list<Packet> GetPacketList();

	// Gets the last number of packets
	std::list<Packet> GetNLastPackets(int N);

	// Get a copy of all the current connections in a list
	std::list <Connection> GetConnections();

	// Constructs the key string for accessing a Connection in connections
	std::string ConstructKeyString(Packet pkt);
	std::string ConstructKeyString(Connection con);

	// Returns true if there is a connection built from the packet arg.
	bool ConnectionExists(Packet pkt);

	// Setter for Packet Limit
	void SetPacketLimit(int max);

	// Creates or Updates connections based on the packet
	void CreateOrUpdateConnection(Packet pkt);

	// Threaded function which sits in the back and constantly checks if the connections
	// have been idle past the timeoutValue
	void CheckTimeout();

	// Sets the internal timeout value in seconds
	void SetTimeout(int seconds);

	// create a filter object 
	Filter* myFilter = new Filter();
	
	// flag for no filter option
	bool noFilter = true;
};
