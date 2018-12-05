#include "pch.h"
#include "CaptureEngine.h"
#include "ThreadManager.h"
#include "GraphicsEngine.h"
#include"Console.h"
#include <string>



//
// NOTE: remember to include WPCAP and HAVE_REMOTE among your
// preprocessor definitions.
//

// checks if user enteres an alias port
bool isAliasPort(std::string port);
//Console Mode
Console con;

// Thread Manager
ThreadManager threadMan;

// Handles all the Packet Capture Logic
CaptureEngine captureEngine(&threadMan);

// Graphics Manager
GraphicsEngine graphics(&captureEngine, &threadMan);

// Controls the "Every-Second" Print Thread 
bool continueSecondUpdate = true;

// Threaded function declaration.
void ThreadPrint();

// Holds taget IP
std::string targetIP="";

// temp choice target
std::string choice;

// type of filter
int typeOfFilter = 0;

int main(int argc, char **argv)
{
	// initialize map
	captureEngine.myFilter->setMapPort();

	//Configuration Menu 
	int menu_choice;
	bool configuring = true, interfaceSet = false;
	while (configuring) {
		std::cout << "Select Menu Mode: \n (1) Set Packet Limit \n (2) Set Connection Timeout \n (3) Set Network Interface \n (4) Set Console \n (5) Start Capture \n (6) Filter \n";
		std::cin >> menu_choice;
		switch (menu_choice)
		{
		case 1: // Set Packet Limit
			int packet_limit;
			std::cout << "Select the max packet limit \n";
			std::cin >> packet_limit;
			captureEngine.SetPacketLimit(packet_limit);
			break;
		case 2: // Set Connection Timeout
			// Set the Connection Timeout in Seconds
			int timeout;
			std::cout << "Select the connection timeout limit (sec) \n";
			std::cin >> timeout;
			captureEngine.SetTimeout(timeout);
			break;
		case 3: 
			// Select Network Interface
			captureEngine.SelectInterface();
			interfaceSet = true;
			break;
		case 4: 
			// Set Console output
			int console_choice;
			std::cout << "Select Console Mode: \n (1) Live Stream \n (2) Connections ";
			std::cin >> console_choice;

			switch (console_choice)
			{
				case 1: // Live Stream
					captureEngine.SetConsoleMode(LiveStream);
					break;
				case 2: // Connections
					captureEngine.SetConsoleMode(ConnectionsMade);
					break;
			}
			break;
		case 5: // Start Capture 
			if (!interfaceSet) {
				std::cout << "You need to set the capture interface first!\n\r";
				break;
			}
			
			// Using &captureEngine as the object reference, start the CaptureEngine::Capture method.
			// The &CaptureEngine::Capture is a reference to the class method.
			// This threading example does not pass arguments.
			threadMan.Threads[threadMan.ThreadCount++] = std::thread(&CaptureEngine::Capture, &captureEngine);

			graphics.StartGLWindow();

			break;
		case 6:
			// Enter Filter
			std::cout << "	Enter 1 for Local IP\n	Enter 2 for Destination IP\n	Enter 3 for Local Port\n	Enter 4 for Destination Port\n";
			std::cin >> typeOfFilter;
			switch (typeOfFilter)
			{
				case 1: // set Local IP
					std::cout << "Enter IP \n";
					std::cin >> choice;

					captureEngine.myFilter->SetLocalTargetIP(choice);
					captureEngine.noFilter = false;
					break;
				case 2: // set Destionation IP
					std::cout << "Enter IP \n";
					std::cin >> choice;

					captureEngine.myFilter->SetDestTargetIP(choice);
					captureEngine.noFilter = false;
					break;
				case 3: // set Local Port
					std::cout << "Enter IP or Port number\n";
					std::cin >> choice;

					if (isAliasPort(choice))
						choice = captureEngine.myFilter->GetLocalPortfromMap(choice);
					captureEngine.myFilter->SetLocalTargetPort(choice);
					captureEngine.noFilter = false;
					break;
				case 4: // Set Destination Port
					std::cout << "Enter IP or Port number\n";
					std::cin >> choice;

					if (isAliasPort(choice))
						choice = captureEngine.myFilter->GetLocalPortfromMap(choice);
					captureEngine.myFilter->SetDestTargetPort(choice);
					captureEngine.noFilter = false;
					break;
				default:
					std::cout << "Answer out of scope\n";
					break;
			}
			break;
		default:
			std::cout << "Answer out of scope\n";
			break;
		}
	}
}
	

bool isAliasPort(std::string port)
{
// Port number will be consider from 0 - 9
// else its an alias
	bool isAlias = true;

	if ((int)port[0] > 47 && (int)port[0] < 58)
		isAlias = false;

	return isAlias;

}
