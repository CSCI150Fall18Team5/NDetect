#include "pch.h"
#include "CaptureEngine.h"
//
// NOTE: remember to include WPCAP and HAVE_REMOTE among your
// preprocessor definitions.
//


// Handles all the Packet Capture Logic
CaptureEngine captureEngine;

// Mutual Exclusion
// Used to give one thread exclusive operation, which prevent code interleaving
std::mutex mux;

// Threaded function declaration.
void ThreadPrint();
// Ctrl-C handler
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);

int main(int argc, char **argv)
{
	// Thread test integration

	/*
		TODO:
			Write functions to sort Packets into Connections

	*/

	// Configures the program to handle CTRL+C and other events when focused on the console.
	SetConsoleCtrlHandler(CtrlHandler, true);

	// Displays the Interfaces available to capture packets.
	// Records the user choice
	captureEngine.SelectInterface();
	
	// Set the capture mode
	captureEngine.SetCaptureMode(0);
	
	// Set the Console output mode
	captureEngine.SetConsoleMode(LiveStream);

	// Set the Live Stream display
	// RawData = Show Packet Data
	// HeaderOnly = Show only the Packet Header
	captureEngine.SetLiveStreamDisplay(HeaderOnly);

	// Testing threading with another local method.
	std::thread secondCounter(ThreadPrint);
	
	// Using &captureEngine as the object reference, start the CaptureEngine::Capture method.
	// The &CaptureEngine::Capture is a reference to the class method.
	// This threading example does not pass arguments.
	std::thread captureThread(&CaptureEngine::Capture, &captureEngine);

	// Start the capture
	// captureEngine.Capture();
	
	// This message will display immediately after choosing the 
	std::cout << "Moved past the Capture() method.\n\r";

	// Wait for the thread to finish before proceeding.
	captureThread.join();
	secondCounter.join();
}


// Example of how
void ThreadPrint() 
{
	
	for (int i = 0; i < 1000; i++)
	{
		// Lock the Mutex, meaning that other threads will give this thread exclusive execution
		mux.lock();
		printf("\n\r %i Seconds have passed since we started capturing, and %i packets were captured. \n\n", i, captureEngine.GetPacketList().size());
		// Unlock mutex, now all threads can operate besides each other.
		mux.unlock();
		Sleep(1000);
	}

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
		std::cout << "CTRL+C pressed, exiting...";
		Sleep(1500);
		exit(0);

		// CTRL-CLOSE: confirm that the user wants to exit. 
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
