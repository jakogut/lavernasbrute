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

void Director::setWorkerPtr(processingPath* worker, int id)
{
	workers[id] = worker;
}

void Director::reassignKeyspace(processingPath *worker)
{
	if(worker == workers[worker->getThreadID()])
	{
		int id = 0;

		// Find the worker with the largest remaining section of the keyspace
		for(int i = 0; i < numWorkers; i++)
		{
			if(getKeyspaceSize(i) < getKeyspaceSize(id))
			{
				id = i;
			}
		}

		// Split the remaining section of the keyspace, and give it to the idle worker
		worker->moveKeyspaceEnd(workers[id]->getKeyspaceEnd());
		workers[id]->moveKeyspaceEnd((workers[id]->getKeyspaceEnd() - workers[id]->getKeyLocation()) / 2);

		worker->moveKeyspaceBegin(workers[id]->getKeyspaceEnd() + 1);
		worker->moveKeylocation(worker->getKeyLocation());

		std::cout << "Keyspace relocated" << std::endl;
	}
}

unsigned long long Director::getKeyspaceSize(int id)
{
	processingPath* worker = workers[id];
	return worker->getKeyspaceEnd() - worker->getKeyspaceBegin();
}

unsigned long long Director::getKeyspaceSize(processingPath* worker)
{
	return worker->getKeyspaceEnd() - worker->getKeyspaceBegin();
}