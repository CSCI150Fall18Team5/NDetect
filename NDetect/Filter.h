#pragma once
#include "pch.h"
#include "DTOs.h"
#include <string>
#include <iostream>



class Filter 
{
	// Target IP from the user
	std::string targetLocalIP = "";
	std::string targetDestIP = "";
	int targetLocalPort = 0;
	int targetDestPort = 0;


public:
	Filter();
	Filter(std::string targetLocalIP, std::string targetDestIP, int targetLocalPort, int targetDestPort);
	~Filter();

	// Set ters 
	void SetLocalTargetIP(std::string);
	void SetDestTargetIP(std::string);
	void SetLocalTargetPort(int);
	void SetDestTargetPort(int);

	// Getters 
	std::string GetLocalTargetIP();
	std::string GetDestTargetIP();
	int GetLocalTargetPort();
	int GetDestTargetPort();

	// Method to show Target IP
	void ShowTargetIP();
};

