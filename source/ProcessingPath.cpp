//Part of Laverna's Brute

#include "ProcessingPath.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

std::map<std::string, std::string> processingPath::targets;
int processingPath::maxChars = 0;
int processingPath::totalThreads = 0;

////////////////////////////////////////////

processingPath::processingPath() 
{
	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	integerToKeyLookup = masterThread::getLookup();

	//Make sure the keyspace size is within the supported limits
	if(maxChars < 1 || maxChars > 14)
	{
		maxChars = 8;
	}
}

processingPath::~processingPath()
{
	integerToKeyLookup = NULL;
}

void processingPath::pushTarget(std::string input)
{
	targets[input.substr(0, 5)] = input;
}

void processingPath::setMaxChars(int input)
{
	maxChars = input;
}

void processingPath::setTotalThreads(int input)
{
	totalThreads = input;
}

int processingPath::getNumTargets()
{
	return targets.size();
}

std::map<std::string, std::string> processingPath::getTargets()
{
	return targets;
}