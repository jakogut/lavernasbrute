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

	startKeyspace = (long long)((pow((double)62, maxChars) / totalThreads) * id);
	endKeyspace = startKeyspace + (long long)(pow((double)62, maxChars) / totalThreads);

	//Assign a unique portion of the keyspace to the thread
	keyLocation = startKeyspace;
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	string Target = getTarget();

	if(linearSearch)
	{
		do
		{
			string tempString = "";
			tempString = integerToKey(keyLocation);
			keyLocation++;

			if(mNTLM->getNTLMHash(tempString) == Target)
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
	else
	{
		do
		{
			string tempString = "";
			tempString = generateRandString(4);

			if(mNTLM->getNTLMHash(tempString) == Target)
			{
				masterThread::setCrackedPassword(tempString);
				masterThread::setSuccess(true);
				break;
			}
			else
			{
				masterThread::incrementIterations();
			}

		} while(!masterThread::getSuccess());
	}
}