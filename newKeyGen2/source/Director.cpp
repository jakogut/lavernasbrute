//Part of Laverna's Brute

#include "Director.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

processingPath** Director::workerPtrArray = 0;
int Director::numWorkers = 0;

////////////////////////////////////////////

Director::Director()
{
	numWorkers = masterThread::getNumWorkers();

	workerPtrArray = new processingPath*[numWorkers];

	for(int i = 0; i < numWorkers; i++)
		workerPtrArray[i] = NULL;
}

Director::Director(unsigned long long beginKeyspace, unsigned long long keyspaceEnd)
{
	delete [] workerPtrArray;
}

Director::~Director()
{
}

void Director::operator()()
{
	boost::posix_time::seconds updateInterval(1);
	unsigned long long totalIterations;

	while(!masterThread::getSuccess())
	{
		boost::this_thread::sleep(updateInterval);
		totalIterations = 0;

		for(int i = 0; i < numWorkers; i++)
			std::cout << workerPtrArray[i]->getThreadID() << std::endl;

		masterThread::setIterations(totalIterations);
	}
}

processingPath* Director::getWorkerPtr(int id)
{
	return workerPtrArray[id];
}

void Director::manageWorker(processingPath* worker)
{
	int threadID = worker->getThreadID();

	if(workerPtrArray[threadID] == NULL)
		workerPtrArray[threadID] = worker;
	else
		std::cerr << "Worker ptr double assignment attempted!" << std::endl;
}

bool Director::reassignKeyspace(processingPath* worker)
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
		worker->moveKeyspaceEnd(workerPtrArray[id]->getKeyspaceEnd());
		workerPtrArray[id]->moveKeyspaceEnd((workerPtrArray[id]->getKeyspaceEnd() - workerPtrArray[id]->getKeyLocation()) / 2);

		worker->moveKeyspaceBegin(workerPtrArray[id]->getKeyspaceEnd() + 1);
		worker->moveKeylocation(worker->getKeyLocation());

		std::cout << workerPtrArray[id]->getKeyLocation() << " -- " << workerPtrArray[id]->getKeyspaceEnd() << std::endl;

		return true;
	}
	else
	{
		return false;
	}
}

void updateIterations()
{
}

unsigned long long Director::getRemainingKeyspace(int id)
{
	processingPath* worker = workerPtrArray[id];
	return worker->getKeyLocation() - worker->getKeyspaceBegin();
}

unsigned long long Director::getRemainingKeyspace(processingPath* worker)
{
	return worker->getKeyLocation() - worker->getKeyspaceBegin();
}