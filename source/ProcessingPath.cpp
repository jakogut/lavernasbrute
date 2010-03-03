//Part of Laverna's Brute

#include "ProcessingPath.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

targetMap processingPath::targets;
boost::mutex processingPath::targetsMutex;
int processingPath::maxChars = 0;

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

void processingPath::removeTarget(targetMap::iterator it)
{
	boost::mutex::scoped_lock lock(targetsMutex);
	targets.erase(it);
}