#include "pch.h"
#include "CaptureEngine.h"
#include "GraphicsEngine.h"
#include "ThreadManager.h"
#include <string>

//
// NOTE: remember to include WPCAP and HAVE_REMOTE among your
// preprocessor definitions.
//

// checks if user enteres an alias port
bool isAliasPort(std::string port);
// Create our Master of Threads
ThreadManager threadMan;

// Handles all the Packet Capture Logic
CaptureEngine captureEngine(&threadMan);

// Graphics Engine performs all the visual representation
GraphicsEngine graphicsEngine(&captureEngine, &threadMan);

// Thread holder
std::thread programThreads[10];
int threadCount = 0;

// Mutual Exclusion
// Used to give one thread exclusive operation, which prevent code interleaving
std::mutex mux;

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


void JoinThreads() {

	// Wait for all threads to finish before proceeding.
	for (int i = 0; i < threadCount; i++)
	{
		// Threads may have been detached when CTRL+C was triggered.
		// Only join joinable threads.
		if (programThreads[i].joinable())
		{
			programThreads[i].join();
		}
	}

}


int main(int argc, char **argv)

{
	// initialize map
	captureEngine.myFilter->setMapPort();

	// Configures the program to handle CTRL+C and other events when focused on the console.
	SetConsoleCtrlHandler(CtrlHandler, true);
	bool CaptureOn = true;
	bool GraphicsOn = false;

	// Initialize GLUT Framework
	glutInit(&argc, argv);

	// Test the map
  //	while (true)
  //	{
  //		std::string temp;
  //		std::cout << "enter port alias\n";
  //		std::cin >> temp;
  //		std::cout << "is it an alias? " << isaliasport(temp)<<"\n";//		std::cout << captureengine.myfilter->getlocalportfrommap(temp)<<"\n";
  //	}

	// Enter Filter
	std::cout << "Do you want to apply a filter (yes/no)? \n";
	std::cin >> choice;
		
	if (choice == "yes")
	{
		std::cout << "	Enter 1 for Local IP\n	Enter 2 for Destination IP\n	Enter 3 for Local Port\n	Enter 4 for Destination Port\n";
		std::cin >> typeOfFilter;
		std::cout << "Enter IP or Port number\n";
	if (CaptureOn) {

		// Enter Filter
		std::cout << "Do you want to apply a filter (yes/no)? \n";
		std::cin >> choice;

		if (choice == "yes")
		{
			case 1: // set Local IP
			std::cout << "Enter 1 for Local IP\n	Enter 2 for Destination IP\n	Enter 3 for Local Port\n	Enter 4 for Destination Port\n";
			std::cin >> typeOfFilter;
			std::cout << "Enter IP or Port number";
			std::cin >> choice;
			switch (typeOfFilter)
			{
			case 1:
				captureEngine.myFilter->SetLocalTargetIP(choice);
				break;
			case 2: // set Destionation IP
				captureEngine.myFilter->SetDestTargetIP(choice);
				break;
			case 3: // set Local Port
				if(isAliasPort(choice))
					choice = captureEngine.myFilter->GetLocalPortfromMap(choice);
				captureEngine.myFilter->SetLocalTargetPort(choice);
				break;
			case 4: // Set Destination Port
				if(isAliasPort(choice))
					choice = captureEngine.myFilter->GetLocalPortfromMap(choice);
				captureEngine.myFilter->SetDestTargetPort(choice);
			case 4:
				//	captureEngine.myFilter->SetDestTargetPort(choice);
				break;
		default:
			std::cout << "Answer out of scope\n";
			break;
			default:
				std::cout << "Answer out of scope\n";
				break;
			}
		}
	}
	else {
		captureEngine.noFilter = true;
	}
		
	// Set the capture mode
	captureEngine.SetCaptureMode(0);
		else {
			captureEngine.noFilter = true;
		}

		// Set the capture mode
		captureEngine.SetCaptureMode(0);
	
		// Set the Connection Timeout in Seconds
		captureEngine.SetTimeout(20);

	// Select the Interfeace
	captureEngine.SelectInterface();

	// Set the Connection Timeout in Seconds
	captureEngine.SetTimeout(5);
		// Set the Console output mode
		captureEngine.SetConsoleMode(ConnectionsMade);

		// Set the Live Stream display
		// RawData = Show Packet Data
		// HeaderOnly = Show only the Packet Header
		captureEngine.SetLiveStreamDisplay(HeaderOnly);

		// Select the Interface
		captureEngine.SelectInterface();

		// Using &captureEngine as the object reference, start the CaptureEngine::Capture method.
		// The &CaptureEngine::Capture is a reference to the class method.
		// This threading example does not pass arguments.
		threadMan.Threads[threadMan.ThreadCount++] = std::thread(&CaptureEngine::Capture, &captureEngine);
		
		// Testing threading with another local method.
		// programThreads[threadCount++] = std::thread(ThreadPrint);
	}

	graphicsEngine.StartGLWindow();
	
}
	
// Example of how to use the Packet data to generate messages
void ThreadPrint() 
{
	int secondsPassed = 1;
	int packetsCaptured = captureEngine.GetPacketList().size();
	int packetsPerSecond = 0;
	while(continueSecondUpdate)
	{
		// Lock the Mutex, meaning that other threads will give this thread exclusive execution while locked.
		mux.lock();
		packetsCaptured = captureEngine.GetPacketList().size(); 
		packetsPerSecond = ((packetsCaptured / secondsPassed) > 0) ? (packetsCaptured / secondsPassed) : 0;
		printf("\n\r %i Seconds have passed since we started capturing, and %i packets were captured at %d packets/sec. \n\n", secondsPassed, packetsCaptured, packetsPerSecond);
		// Unlock mutex, now all threads can operate besides each other.
		mux.unlock();
		Sleep(1000);
		secondsPassed++;
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

/*
	Sourced from: https://docs.microsoft.com/en-us/windows/console/registering-a-control-handler-function
	Handles when CTRL+C and other events are pressed during execution.
*/
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		// Lock this thread
		mux.lock();	
		// Stop the Capturing Output
		captureEngine.SetContinueCapturing(false);
		// Stop the Update message
		continueSecondUpdate = false;
		// Detach all threads so they can safely be deleted.
		for (int i = 0; i < threadCount; i++)
		{
			// Kill the other threads
			programThreads[i].detach();
		}
		std::cout << " \n\n CTRL+C pressed, exiting... \n\n\n";
		Sleep(1500);
		// Unlock and exit
		mux.unlock();		
		exit(0);

	case CTRL_CLOSE_EVENT:
		Beep(600, 200);
		printf("Ctrl-Close event\n\n");
		return TRUE;

		// Pass other signals to the next handler. 
	case CTRL_BREAK_EVENT:
		Beep(900, 200);
		printf("Ctrl-Break event\n\n");
		return TRUE;

	case CTRL_LOGOFF_EVENT:
		Beep(1000, 200);
		printf("Ctrl-Logoff event\n\n");
		return FALSE;

	case CTRL_SHUTDOWN_EVENT:
		Beep(750, 500);
		printf("Ctrl-Shutdown event\n\n");
		return FALSE;

	default:
		return FALSE;
	}
}