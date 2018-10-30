#include "pch.h"
#include "CaptureEngine.h"
#include <thread>
#include <iostream>

#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <winsock.h>
#endif
//
// NOTE: remember to include WPCAP and HAVE_REMOTE among your
// preprocessor definitions.
//

CaptureEngine cap;

int count = 0;
// Test to run threads 
void countBackground()
{
	std::cout << "Hello Concurrent World\n";
	while (count < 1000)
	{
		Sleep(10);
		std::cout << "Thread 1 Count: " << count << std::endl;
		count++;
	}

}
void display()
{
	while (count != 1000)
	{
		std::cout << "thread 2 Count: " << count << std::endl;
	}

}

void RunTraffic()
{
	/*
		TODO
			Make Capture Packets through CaptureEngine Class
			Make run in a mode that displays only the interfaces
			Code a way to pass down the arguments of the program into the capture
				Select Interface from args
			Show captured packets as Source/Dest IPS, MACs, and Ports
			Show Data in ACSII

	*/

	cap.SelectInterface();
	// Capture Mode: 1 = Promiscuous, 0 = Normal
	// Promiscuous means that all packets, even those not destined for the
	// host computer are captured. This provides 
	cap.Capture(1);
}

void PrintStuff()
{
	while (count != 500)
		std::cout<< "On the third thread \n";
}

int main(int argc, char **argv)
{
	// Thread test integration

	std::thread t(countBackground);
	std::thread t2(display);
	std::thread getTraffic(RunTraffic);
	std::thread printStuff(PrintStuff);




	// Join the threads
	printStuff.join();
	getTraffic.join();
	t2.join();
	t.join();
	





}