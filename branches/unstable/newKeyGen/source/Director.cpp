//Part of Laverna's Brute

#include "Director.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

std::map<int, processingPath*> Director::workerPtrMap;
int Director::numWorkers = 0;
int Director::workersInitialized = 0;

////////////////////////////////////////////

Director::Director()
{
}

Director::Director(unsigned long long beginKeyspace, unsigned long long keyspaceEnd)
{
	// Search a given section of the keyspace, rather than the entire thing.
}

Director::~Director()
{
}

void Director::operator()()
{
	numWorkers = masterThread::getNumWorkers();

	// Wait for all workers to be initialized.
	while(workerPtrMap.size() != numWorkers)

	// Start updating the masterThread.
	updateMasterThread();
}

void Director::updateMasterThread()
{
	boost::posix_time::seconds updateInterval(1);
	unsigned long long totalIterations;

	while(!masterThread::getSuccess())
	{
		boost::this_thread::sleep(updateInterval);
		totalIterations = 0;

		for(int i = 0; i < numWorkers; i++)
			totalIterations += workerPtrMap[i]->getKeyLocation() - workerPtrMap[i]->getKeyspaceBegin();

		masterThread::setIterations(totalIterations);
	}
}

processingPath* Director::getWorkerPtr(int id)
{
	return workerPtrMap[id];
}

void Director::manageWorker(processingPath* worker)
{
	workerPtrMap[worker->getThreadID()] = worker;
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
		worker->moveKeyspaceEnd(workerPtrMap[id]->getKeyspaceEnd());
		workerPtrMap[id]->moveKeyspaceEnd((workerPtrMap[id]->getKeyspaceEnd() - workerPtrMap[id]->getKeyLocation()) / 2);

		worker->moveKeyspaceBegin(workerPtrMap[id]->getKeyspaceEnd() + 1);
		worker->moveKeylocation(worker->getKeyLocation());

		std::cout << workerPtrMap[id]->getKeyLocation() << " -- " << workerPtrMap[id]->getKeyspaceEnd() << std::endl;

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
	processingPath* worker = workerPtrMap[id];
	return worker->getKeyLocation() - worker->getKeyspaceBegin();
}

unsigned long long Director::getRemainingKeyspace(processingPath* worker)
{
	return worker->getKeyLocation() - worker->getKeyspaceBegin();
}
