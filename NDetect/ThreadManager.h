#pragma once
#include "pch.h"

// Making the
class ThreadManager
{
public:

	// Controls if threads should continue to run. Usually false when program shutting down.
	bool threadsContinue = true;
	// Set when EndThreads called
	bool threadsEnded = false;

	// Signaler for other threads to resume
	std::condition_variable condVar;

	// Mutual Exclusion, used to stop multiple threads
	std::mutex muxConnections;
	std::mutex muxPackets;

	// All the threads we need
	static const int ThreadMax = 10;
	int ThreadCount = 0;
	std::thread Threads[ThreadMax];

	// Const/Dest
	ThreadManager();
	~ThreadManager();
	
	// Tries to join or detach all threads.
	void EndThreads();

};

