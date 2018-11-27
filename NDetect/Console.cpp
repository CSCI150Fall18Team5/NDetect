#include "pch.h";
#include "Console.h";

Console::Console() {
}

Console::~Console() {

}
void Console::Console_Diplay() {
	// Console Mode 

	int console_choice;
	std::cout << "Select Console Mode: (1) Live Stream \n (2) Statistics \n (3) Combo \n (4)Connection \n";
	std::cin >> console_choice;
	switch (console_choice)
	{
	case 1: // Live Stream

		break;
	case 2: // Statistics

		break;
	case 3: //  Combo

		break;
	case 4: // Connection

		break;
	default:
		std::cout << "Answer out of scope\n";
		break;
	}
}
void Console::Live_Stream() {

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
