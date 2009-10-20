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
	static void setWorkerPtr(processingPath* worker, int id);

	static void reassignKeyspace(processingPath* worker);

protected:

	static unsigned long long getKeyspaceSize(int id);
	static unsigned long long getKeyspaceSize(processingPath* worker);

	static processingPath** workers;

	static bool server;
	static int numWorkers;
};

#endif