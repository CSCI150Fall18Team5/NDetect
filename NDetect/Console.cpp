#include "pch.h";
#include "Console.h";
#include "CaptureEngine.h"
#include "ThreadManager.h"
#include "GraphicsEngine.h"



Console::Console() {
}

Console::~Console() {

}
void Console::Console_Diplay() {
	// Console Mode 

	int console_choice;
	std::cout << "Select Console Mode: \n (1) Live Stream \n (2) Statistics \n (3) Combo \n (4)Connection \n";
	std::cin >> console_choice;
	
	switch (console_choice)
	{
	case 1: // Live Stream
		Live_Stream();
		break;
	case 2: // Statistics
		std::cout << "Not completed yet.....Oopps \n";
		break;
	case 3: //  Combo
		std::cout << "Only Live Stream working at this point \n";
		break;
	case 4: // Connection

		break;
	default:
		std::cout << "Answer out of scope\n";
		break;
	}
}
void Console::Live_Stream() {
	ThreadManager threading;
	CaptureEngine captureEngine(&threading);
	captureEngine.myFilter->setMapPort();
	GraphicsEngine display(&captureEngine, &threading);

	captureEngine.SetCaptureMode(0);
	captureEngine.SelectInterface();
	captureEngine.SetConsoleMode(ConnectionsMade);
	captureEngine.SetLiveStreamDisplay(HeaderOnly);
	threading.Threads[threading.ThreadCount++] = std::thread(&CaptureEngine::Capture, &captureEngine);
	display.StartGLWindow();
}
void Console::Statistics() {
	//Will call the class Statistics that will display the data. 
	//Statistics Stats ();
	//data=Stats.getData; 
	/*
		for(int i=0; i<data.size();i++){
		printf("Statitcs data"+ data[i];
		}
	*/
}
void Console::Combo_Diplay() {
	Live_Stream();
	//Statistics();

}
void Console::Connections() {

}


//void Console::Filter() {}