#include "pch.h"
#include "Filter.h"
#include "DTOs.h"
#include <string>
#include <iostream>

Filter::Filter()
{
}

Filter::Filter(std::string targetLocalIP, std::string targetDestIP, int targetLocalPort, int targetDestPort)
{
	this->targetLocalIP = targetLocalIP;
	this->targetDestIP = targetDestIP;
	this->targetLocalPort = targetLocalPort;
	this->targetDestPort = targetDestPort;
}


Filter::~Filter()
{
}

void Filter::SetLocalTargetIP(std::string IP)
{
	targetLocalIP = IP;
}

void Filter::SetDestTargetIP(std::string IP)
{
	targetDestIP = IP;
}

void Filter::SetLocalTargetPort(int port)
{
	targetLocalPort = port;
}

std::string Filter::GetLocalTargetIP()
{
	return std::string(targetLocalIP);
}

std::string Filter::GetDestTargetIP()
{
	return std::string(targetDestIP);
}

int Filter::GetLocalTargetPort()
{
	return targetLocalPort;
}

int Filter::GetDestTargetPort()
{
	return targetDestPort;
}


// Method to show Target IP
void Filter::ShowTargetIP()
{
	if(targetLocalIP != "")
		std::cout << "Target IP Local: " << targetLocalIP << " \t|\n\r";
	if( targetDestIP != "")
		std::cout << "Target IP Destination: " << targetDestIP << " \t|\n\r";
	if( targetLocalPort != 0)
		std::cout << "Target port Local: " << targetDestPort << " \t|\n\r";
	if(targetDestPort != 0)
		std::cout << "Target port Destination: " << targetDestPort << " \t|\n\r";

}