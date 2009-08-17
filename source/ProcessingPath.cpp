//Part of Laverna's Brute

#include "ProcessingPath.h"

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
	strcpy(target, (char*)input.c_str());
}

void processingPath::setTarget(char* input)
{
	strcpy(target, input);
	target[32] = '\0';
}

char* processingPath::getTarget()
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