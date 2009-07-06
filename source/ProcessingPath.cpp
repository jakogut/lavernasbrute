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
std::string processingPath::integerToKey(unsigned long long* location)
{
	unsigned long long num = *location;
	std::string result;

	while(num > 0)
	{
		result += charset[num % (charsetLength + 1)];
		num /= (charsetLength + 1);
	}

	return result;
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