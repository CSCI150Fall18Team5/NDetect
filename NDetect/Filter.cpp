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

void Filter::SetLocalTargetPort(std::string port)
{
	targetLocalPort = port;
}

void Filter::SetDestTargetPort(std::string port)
{
	targetDestPort = port;
}

std::string Filter::GetLocalTargetIP()
{
	return std::string(targetLocalIP);
}

std::string Filter::GetDestTargetIP()
{
	return std::string(targetDestIP);
}

std::string Filter::GetLocalTargetPort()
{
	return std::string(targetLocalPort);
}

std::string Filter::GetDestTargetPort()
{
	return std::string(targetDestPort);
}


// Method to show Target IP
void Filter::ShowTargetIP()
{
	if(targetLocalIP != "")
		std::cout << "Target IP Local: " << targetLocalIP << " \t|\n\r";
	if( targetDestIP != "")
		std::cout << "Target IP Destination: " << targetDestIP << " \t|\n\r";
	if( targetLocalPort != "")
		std::cout << "Target port Local: " << targetDestPort << " \t|\n\r";
	if(targetDestPort != "")
		std::cout << "Target port Destination: " << targetDestPort << " \t|\n\r";

}