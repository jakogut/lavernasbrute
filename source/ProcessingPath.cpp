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
	else if (maxChars > 10)
	{
		maxChars = 10;
	}
}

processingPath::~processingPath()
{
}

void processingPath::setTarget(std::string input)
{
	strcpy(target, (char*)input.c_str());
}

void processingPath::setTarget(char* input)
{
	strcpy(target, input);
	target[32] = 0;
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