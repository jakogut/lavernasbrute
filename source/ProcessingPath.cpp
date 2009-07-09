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

//Convert the integer key location to a text string
void processingPath::integerToKey(unsigned long long* location, std::string* output)
{
	unsigned long long num = *location;
	*output = "";

	while(num > 0)
	{
		*output += charset[num % (charsetLength + 1)];
		num /= charsetLength + 1;
	}
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