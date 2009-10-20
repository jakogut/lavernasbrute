//Part of Laverna's Brute

#ifndef DIRECTOR_H_
#define DIRECTOR_H_

#include <boost/thread/thread.hpp>
#include "ProcessingPath.h"

class Director
{
public:

	Director();
	Director(unsigned long long beginKeyspace, unsigned long long keyspaceEnd);

	~Director();

	void operator()();

	static processingPath* getWorkerPtr(int id);
	static void setWorkerPtr(processingPath* worker);

	// The return value is whether or not the director was able to create new work for the idle thread
	static bool reassignKeyspace(processingPath* worker);

protected:

	static unsigned long long getRemainingKeyspace(int id);
	static unsigned long long getRemainingKeyspace(processingPath* worker);

	static processingPath** workers;

	static bool server;
	static int numWorkers;
};

#endif