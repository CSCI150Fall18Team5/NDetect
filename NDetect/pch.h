#ifndef PCH_H
#define PCH_H

// Exception Handling
#include <exception>

// Needed for threading
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

// Used to measure time
#include <ctime>

// File IO
#include <fstream>
#include <iostream>

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
#include <vector>

// Needed for some of the Capture Engine code
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <winsock.h>
#endif

// Needed for GLUT
#include <string.h>
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#endif //PCH_H
