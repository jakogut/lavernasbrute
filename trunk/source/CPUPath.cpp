//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	mRand = new MTRand();
	mNTLM = new NTLM();

	//Multiplying the time by the thread id ensures that the RNG is seeded uniquely for ever thread of every processing path
	mRand->seed((int)(time(NULL) * id));

	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	startKeyspace = ((pow((unsigned long double)charsetLength, maxChars) / totalThreads) * id);
	endKeyspace = startKeyspace + (pow((unsigned long double)charsetLength, maxChars) / totalThreads);

	//Assign a unique portion of the keyspace to the thread
	keyLocation = startKeyspace;
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	std::string tempString;

	do
	{
		tempString = integerToKey(&keyLocation);
		keyLocation++;

		if(mNTLM->getNTLMHash(tempString) == target)
		{
			masterThread::setCrackedPassword(tempString);
			masterThread::setSuccess(true);
			break;
		}
		else
		{
			masterThread::incrementIterations();
		}

	} while(!masterThread::getSuccess() && keyLocation < endKeyspace);
}