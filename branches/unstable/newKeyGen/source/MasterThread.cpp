//Part of Laverna's Brute

#include "MasterThread.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

bool masterThread::success = 0;
bool masterThread::silent = 0;
int masterThread::numWorkers = 0;
int masterThread::remainingTargets = 0;
std::vector< std::pair<std::string, std::string> > masterThread::results;
boost::mutex masterThread::stdoutMutex;
characterSet masterThread::charset;
std::map<int, processingPath*> masterThread::workerPtrMap;


////////////////////////////////////////////

masterThread::masterThread()
{
	// Start the clock
	startTime = time(NULL);

	processingPath::setCharset(&charset);
}

masterThread::~masterThread()
{
}

void masterThread::operator()()
{
	boost::posix_time::seconds updateInterval(1);
	std::cout.precision(3);

	while(workerPtrMap.size() != (unsigned int)masterThread::getNumWorkers()) 
		boost::this_thread::sleep(updateInterval);

	do
	{
		boost::this_thread::sleep(updateInterval);

		if(!silent)
		{
			boost::mutex::scoped_lock lock(stdoutMutex);

			std::cout.flush();

			std::cout << "\rAverage speed: " << ((getIterations() / (time(NULL) - startTime)) / 1000000.0f) << " M keys/s"
			   << "\tHashes Remaining: " << remainingTargets;

			std::cout.flush();
		}

	} while(!success);

	printStatistics();
}

void masterThread::printStatistics()
{
	time_t endTime = time(NULL);
	time_t totalTime = (endTime - startTime);

	time_t seconds = totalTime % 60;
	totalTime /= 60;

	time_t minutes = totalTime % 60;
	totalTime /= 60;

	time_t hours = totalTime % 24;
	totalTime /= 24;

	time_t days = totalTime;

	std::cout.precision(3);

	std::cout << "\n\nThe run has completed!"
		<< "\nAttack duration: " << getIterations() << " iterations."
		<< "\nCompleted in: " << days << " days, " << hours << " hours, "
		<< minutes << " minutes, and " << seconds << " seconds. ";

	if(silent)
	{
		std::cout << "\n\nAverage speed: " << ((getIterations() / (time(NULL) - startTime)) / 1000000.0f) << " M keys/s" << std::endl;
		std::cout.flush();
	}
	else
	{
		std::cout << std::endl;
	}
}

bool masterThread::getSuccess()
{
	return success;
}

void masterThread::setSuccess()
{
	success = true;
}

int masterThread::getNumWorkers()
{
	return numWorkers;
}

void masterThread::setNumWorkers(int input)
{
	numWorkers = input;
}

void masterThread::setRemainingTargets(int input)
{
	remainingTargets = input;
}

unsigned long long masterThread::getIterations()
{
	unsigned long long totalIterations = 0;

	for(int i = 0; i < numWorkers; i++)
			totalIterations += workerPtrMap[i]->getKeyLocation() - workerPtrMap[i]->getKeyspaceBegin();

	return totalIterations;
}

void masterThread::initCharset(int min, int max, 
							   int charsec0, int charsec1, int charsec2, int charsec3)
{
	charset.init(min, max, charsec0, charsec1, charsec2, charsec3);
}

characterSet* masterThread::getCharset()
{
	return &charset;
}

void masterThread::printResult(std::string hash, std::string plaintext)
{

	boost::mutex::scoped_lock lock(stdoutMutex);

	if(!silent)
	{
		// Clear the last line
		std::cout << "\r";
		std::cout.flush();
	}

	std::cout << hash << " == ";

	std::cout.width(80);

	std::cout << std::left << plaintext << std::endl << std::endl;

	std::cout.flush();
}

bool masterThread::getSilent()
{
	return silent;
}

void masterThread::setSilent(bool input)
{
	silent = input;
}

processingPath* masterThread::getWorkerPtr(int id)
{
	return workerPtrMap[id];
}

void masterThread::manageWorker(processingPath* worker)
{
	workerPtrMap[worker->getThreadID()] = worker;

	// Assign a unique portion of the keyspace to the thread (Based on id)
	unsigned long long keyspaceSize;
	pow<unsigned long long>(charset.length, processingPath::getMaxChars(), keyspaceSize);
	keyspaceSize /= numWorkers;

	worker->moveKeyspaceBegin(keyspaceSize * worker->getThreadID());
	worker->moveKeyspaceEnd(worker->getKeyspaceBegin() + keyspaceSize);

	// Set the key location
	worker->moveKeylocation(worker->getKeyspaceBegin());

	// Start the search
	worker->searchKeyspace();
}

bool masterThread::reassignKeyspace(processingPath* worker)
{
	int id = 0;

	// Find the worker with the largest remaining section of the keyspace
	for(int i = 0; i < numWorkers; i++)
		if(getRemainingKeyspace(i) > getRemainingKeyspace(id))
			id = i;

	if(getRemainingKeyspace(id) > 0)
	{
		// Split the remaining section of the keyspace, and give it to the idle worker
		worker->moveKeyspaceEnd(workerPtrMap[id]->getKeyspaceEnd());
		workerPtrMap[id]->moveKeyspaceEnd((workerPtrMap[id]->getKeyspaceEnd() - workerPtrMap[id]->getKeyLocation()) / 2);

		worker->moveKeyspaceBegin(workerPtrMap[id]->getKeyspaceEnd() + 1);
		worker->moveKeylocation(worker->getKeyLocation());

		return true;
	}
	else
	{
		return false;
	}
}

unsigned long long masterThread::getRemainingKeyspace(int id)
{
	processingPath* worker = workerPtrMap[id];
	return worker->getKeyLocation() - worker->getKeyspaceBegin();
}

unsigned long long masterThread::getRemainingKeyspace(processingPath* worker)
{
	return worker->getKeyLocation() - worker->getKeyspaceBegin();
}
