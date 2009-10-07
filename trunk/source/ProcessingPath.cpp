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
	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	integerToKeyLookup = masterThread::getLookup();

	//Make sure the keyspace size is within the supported limits
	if(maxChars <= 0)
	{
		maxChars = 8;
	}
	else if(sizeof(long) == 4 && maxChars > 8) // A 32-bit system will allow for a keyspace of the size 62^8
	{
		maxChars = 8;
	}
	else if(sizeof(long) == 8 && maxChars > 14) // A 64-bit system will allow for 62^14
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

unsigned long processingPath::pow(unsigned long base, unsigned long power)
{
	unsigned long result = 1;

	for(long i = 0; i < power; i++)
		result *= base;

	return result;
}