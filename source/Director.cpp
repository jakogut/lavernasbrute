//Part of Laverna's Brute

#include "Director.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

processingPath** Director::workers = 0;
int Director::numWorkers = 0;

Director::Director()
{
	numWorkers = masterThread::getNumWorkers();

	workers = new processingPath*[numWorkers];
}

Director::Director(unsigned long long beginKeyspace, unsigned long long keyspaceEnd)
{
}

Director::~Director()
{
}

void Director::operator()()
{
}

processingPath* Director::getWorkerPtr(int id)
{
	return workers[id];
}

void Director::manageWorker(processingPath* worker)
{
	workers[worker->getThreadID()] = worker;
}

bool Director::reassignKeyspace(processingPath *worker)
{
	int id = 0;
	bool success;

	// Find the worker with the largest remaining section of the keyspace
	for(int i = 0; i < numWorkers; i++)
	{
		if(getRemainingKeyspace(i) > getRemainingKeyspace(id))
		{
			id = i;
		}
	}

	if(getRemainingKeyspace(id) > 0)
	{
		// Split the remaining section of the keyspace, and give it to the idle worker
		worker->moveKeyspaceEnd(workers[id]->getKeyspaceEnd());
		workers[id]->moveKeyspaceEnd((workers[id]->getKeyspaceEnd() - workers[id]->getKeyLocation()) / 2);

		worker->moveKeyspaceBegin(workers[id]->getKeyspaceEnd() + 1);
		worker->moveKeylocation(worker->getKeyLocation());

		std::cout << workers[id]->getKeyLocation() << " -- " << workers[id]->getKeyspaceEnd() << std::endl;

		success = true;
	}
	else
		success = false;

	return success;
}

unsigned long long Director::getRemainingKeyspace(int id)
{
	processingPath* worker = workers[id];
	return worker->getKeyLocation() - worker->getKeyspaceBegin();
}

unsigned long long Director::getRemainingKeyspace(processingPath* worker)
{
	return worker->getKeyLocation() - worker->getKeyspaceBegin();
}