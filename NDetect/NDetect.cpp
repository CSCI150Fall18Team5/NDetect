#include "pch.h"
#include "CaptureEngine.h"
//
// NOTE: remember to include WPCAP and HAVE_REMOTE among your
// preprocessor definitions.
//


// Handles all the Packet Capture Logic
CaptureEngine ce;

int main(int argc, char **argv)
{
	// Thread test integration

	/*
		TODO
			Code a way to pass down the arguments of the program into the capture
				Select Interface from args
			Separate the CaptureEngine into its own thread
			Create a data structure to hold each packet

	*/

	// Displays the Interfaces available to capture packets.
	ce.SelectInterface();
	
	// Set the capture mode
	ce.SetCaptureMode(0);
	
	// Set the Console output mode
	ce.SetConsoleMode(LiveStream);

	// Set the Live Stream display
	// True = Show Packet Data
	// False = Show only the Packet Header
	ce.SetLiveStreamDisplay(true);

	// Start the capture
	ce.Capture();

}