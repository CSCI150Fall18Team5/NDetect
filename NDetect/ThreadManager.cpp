#include "pch.h"
#include "ThreadManager.h"


ThreadManager::ThreadManager()
{
}


ThreadManager::~ThreadManager()
{
	EndThreads();
}

void ThreadManager::EndThreads()
{
	if (!threadsEnded) {
		// Tell all threads to stop their loops
		threadsContinue = false;
		// Either join or Detach all threads
		for (int i = 0; i < ThreadCount; i++)
		{
			if (Threads[i].joinable()) {
				Threads[i].join();
			}
			else {
				Threads[i].detach();
			}
		}
		threadsEnded = true;
	}
}

