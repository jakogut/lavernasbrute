//Part of Laverna's Brute

#include "ProcessingPath.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

std::vector<std::string> processingPath::targets;
int processingPath::maxChars = 0;
int processingPath::totalThreads = 0;

////////////////////////////////////////////

processingPath::processingPath() 
{
	charset = masterThread::getCharsetPtr();
	charsetLength = masterThread::getCharsetLength();

	integerToKeyLookup = masterThread::getLookup();

	//Make sure the keyspace size is within the supported limits
	if(maxChars <= 0)
	{
		maxChars = 8;
	}
	else if (maxChars > 14)	
	{
		maxChars = 14;
	}
}

processingPath::~processingPath()
{
	integerToKeyLookup = NULL;
}

void processingPath::pushTarget(std::string input)
{
	targets.push_back(input);
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

std::vector<std::string> processingPath::getTargets()
{
	return targets;
}

unsigned long long processingPath::pow64(unsigned long long base, int power)
{
	unsigned long long result = 1;

	for(int i = 0; i < power; i++)
		result *= base;

	return result;
}