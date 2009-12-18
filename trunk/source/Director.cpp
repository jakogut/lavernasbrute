//Part of Laverna's Brute

#include "Director.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

std::vector<processingPath*> Director::workers;
int Director::numWorkers = 0;

////////////////////////////////////////////

Director::Director()
{
	numWorkers = masterThread::getNumWorkers();
}

Director::Director(unsigned long long beginKeyspace, unsigned long long keyspaceEnd)
{
}

Director::~Director()
{
}

void Director::operator()()
{
	/* We can use this thread in the future to gather data from threads, 
	and write it to an XML file to create persistent runs. */
}

processingPath* Director::getWorkerPtr(int id)
{
	return workers[id];
}

void Director::manageWorker(processingPath* worker)
{
	workers.insert(workers.begin() + worker->getThreadID(), worker);
}

bool Director::reassignKeyspace(processingPath *worker)
{
	int id = 0;

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

		return true;
	}
	else
		return false;
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