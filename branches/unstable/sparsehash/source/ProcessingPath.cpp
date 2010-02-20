//Part of Laverna's Brute

#include "ProcessingPath.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

target_map processingPath::targets;
boost::mutex processingPath::targetsMutex;
int processingPath::maxChars = 0;
int processingPath::numWorkers = 0;

////////////////////////////////////////////

processingPath::processingPath() 
{
	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	//Make sure the keyspace size is within the supported limits
	if(maxChars < 1 || maxChars > 14)
	{
		maxChars = 8;
	}

	numWorkers = masterThread::getNumWorkers();
}

processingPath::~processingPath()
{
}

void processingPath::initializeTargetMap()
{
	int64_pair empty, deleted;

	empty.first = 0, empty.second = 0;
	targets.set_empty_key(empty);

	deleted.first = 1, deleted.second = 1;
	targets.set_deleted_key(deleted);
}

void processingPath::pushTarget(std::string input)
{
	NTLM ntlm;
	targets[ntlm.weakenHash(input)] = input;
}

void processingPath::setMaxChars(int input)
{
	maxChars = input;
}

int processingPath::getMaxChars()
{
	return maxChars;
}

int processingPath::getNumTargets()
{
	return (int)targets.size();
}

void processingPath::removeTarget(target_map::iterator it)
{
	boost::mutex::scoped_lock lock(targetsMutex);
	targets.erase(it);
}