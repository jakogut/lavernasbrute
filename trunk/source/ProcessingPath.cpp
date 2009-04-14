#include "ProcessingPath.h"

processingPath::processingPath() 
{
	mRand = new MTRand();

	//Multiplying the time by the thread id ensures that the RNG is seeded uniquely for ever thread of every processing path
	mRand->seed((int)(time(NULL) * id));

	mNTLM = new NTLM();

	randCharset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();
}

processingPath::~processingPath()
{
}

void processingPath::setTarget(string write)
{
	target = write;
}

void processingPath::useStandardRand(bool write)
{
	randFast = write;
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
			result += randCharset[rand() % charsetLength - 1];
		}
	}
	else
	{
		for(int i = 0; i < length; ++i) 
		{
			result += randCharset[mRand->randInt(charsetLength - 1)];
		}
   }

   return result;
}