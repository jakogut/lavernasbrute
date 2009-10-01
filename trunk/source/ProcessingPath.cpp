//Part of Laverna's Brute

#include "ProcessingPath.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

std::string processingPath::target;
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
	else if (maxChars > 10)	
	{
		maxChars = 10;
	}
}

processingPath::~processingPath()
{
	integerToKeyLookup = NULL;
}

void processingPath::setTarget(std::string input)
{
	target = input;
}

std::string processingPath::getTarget()
{
	return target;
}

void processingPath::setMaxChars(int input)
{
	maxChars = input;
}

void processingPath::setTotalThreads(int input)
{
	totalThreads = input;
}