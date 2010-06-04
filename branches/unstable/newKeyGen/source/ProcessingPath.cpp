//Part of Laverna's Brute

#include "ProcessingPath.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

targetMap processingPath::targets;
boost::mutex processingPath::targetsMutex;
int processingPath::maxChars = 0;
std::string processingPath::hashType;

////////////////////////////////////////////

processingPath::processingPath() 
{
	//Make sure the keyspace size is within the supported limits
	if(maxChars < 1 || maxChars > 10)
	{
		maxChars = 10;
	}
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

void processingPath::pushTarget(std::string& input)
{
	MD4 md4;
	targets[md4.weakenHash(input.c_str())] = input;
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

void processingPath::removeTarget(targetMap::iterator it)
{
	boost::mutex::scoped_lock lock(targetsMutex);
	targets.erase(it);
}

void processingPath::setHashType(std::string type)
{
	hashType = type;
}

unsigned long long processingPath::calculateKeyspaceSize(int charsetLength, int keyLength)
{
	unsigned long long  temp, retval(1);

	for(int i = 0; i < keyLength; i++)
	{
		temp = 1;

		for(int j = 0; j < i; j++)
			temp *= (unsigned long long)charsetLength;

		retval += temp;
	}

	return retval;
}

unsigned long long processingPath::calculateKeyspaceSize()
{
	return calculateKeyspaceSize(masterThread::getCharset()->length, maxChars);
}