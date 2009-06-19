//Part of Laverna's Brute

#include "ProcessingPath.h"

processingPath::processingPath() 
{
	mRand = new MTRand();

	//Multiplying the time by the thread id ensures that the RNG is seeded uniquely for ever thread of every processing path
	mRand->seed((int)(time(NULL) * id));

	mNTLM = new NTLM();

	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	randFast = false;
	linearSearch = true;

	if(maxChars <= 0)
	{
		maxChars = 8;
	}
}

processingPath::~processingPath()
{
}

//Convert an integer to a base 62 key
std::string processingPath::integerToKey(unsigned long long* location)
{
	unsigned long long num = *location;
	std::string result;

	while(num > 0)
	{
		result += charset[num % 62];
		num /= 62;
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

void processingPath::useLinearSearch(bool input)
{
	linearSearch = input;
}