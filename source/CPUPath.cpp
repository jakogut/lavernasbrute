//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	startKeyspace = (long long)((pow((double)charsetLength, maxChars) / totalThreads) * id);
	endKeyspace = startKeyspace + (long long)(pow((double)charsetLength, maxChars) / totalThreads);

	//Assign a unique portion of the keyspace to the thread
	keyLocation = startKeyspace;
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	do
	{
		currentKey = integerToKey(&keyLocation);
		keyLocation++;

		if(ntlm.getNTLMHash(currentKey) == target)
		{
			masterThread::setCrackedPassword(currentKey);
			masterThread::setSuccess(true);
		}
		else
		{
			masterThread::incrementIterations();
		}

	} while(!masterThread::getSuccess() && keyLocation < endKeyspace);
}