#ifndef PCH_H
#define PCH_H

// Exception Handling
#include <exception>

// Needed for threading
#include <thread>
#include <mutex>

// Used to measure time
#include <ctime>

// Usual includes for std, cout, and string
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

// Needed for Packet Capture Engine
#include <pcap.h>
#include <WinSock2.h>

// Data structures
#include <list>
#include <map>

// Needed for some of the Capture Engine code
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <winsock.h>
#endif

#endif //PCH_H