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
	linearSearch = false;

	if(maxChars <= 0)
	{
		maxChars = 8;
	}
}

processingPath::~processingPath()
{
}

void processingPath::setTarget(string write)
{
	target = write;
}

void processingPath::setMaxChars(int write)
{
	maxChars = write;
}

void processingPath::setTotalThreads(int write)
{
	totalThreads = write;
}

void processingPath::useStandardRand(bool write)
{
	randFast = write;
}

void processingPath::useLinearSearch(bool write)
{
	linearSearch = write;
}

string processingPath::getTarget()
{
	return target;
}

//Generate random string of numbers to be converted to text. Who says randomness isn't useful?
string processingPath::generateRandString(int length)
{
   string result = "";

	if(randFast)
	{
		for(int i = 0; i < length; ++i) 
		{
			result += charset[rand() % charsetLength - 1];
		}
	}
	else
	{
		for(int i = 0; i < length; ++i) 
		{
			result += charset[mRand->randInt(charsetLength - 1)];
		}
   }

   return result;
}

string processingPath::integerToKey(long long location)
{
	int num = location;
	string result;

	while(num > 0)
	{
		result += charset[num % 62];
		num /= 62;
	}

	return result;
}