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
	// Switchable thread ending method
	int threadEndMethod = 0;

	if (!threadsEnded) {
		// Tell all threads to stop their loops
		threadsContinue = false;
		// Either join or Detach all threads
		for (int i = 0; i < ThreadCount; i++)
		{
			// Choice between detach all, or try to join.
			if (threadEndMethod = 0) {
				Threads[i].detach();
			}
			else {
				if (Threads[i].joinable()) {
					Threads[i].join();
				}
				else {
					Threads[i].detach();
				}
			}
		}
		threadsEnded = true;
	}
}

