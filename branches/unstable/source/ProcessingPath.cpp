//Part of Laverna's Brute

#include "ProcessingPath.h"

processingPath::processingPath() 
{
	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	if(maxChars <= 0)
	{
		maxChars = 8;
	}
}

processingPath::~processingPath()
{
}

void processingPath::setTarget(char* input)
{
	target = input;
}

char* processingPath::getTarget()
{
	return target;
}

void processingPath::setMaxChars(int input)
{
	maxChars = input;
}

int processingPath::getMaxChars()
{
	return maxChars;
}

void processingPath::setTotalThreads(int input)
{
	totalThreads = input;
}