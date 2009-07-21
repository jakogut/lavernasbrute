//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	startKeyspace = (unsigned long long)((pow((double)charsetLength, maxChars) / totalThreads) * id);
	endKeyspace = startKeyspace + (unsigned long long)(pow((double)charsetLength, maxChars) / totalThreads);

	//Assign a unique portion of the keyspace to the thread
	keyLocation = startKeyspace;

	localProgress = 0;
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	do
	{
		integerToKey(keyLocation, &currentKey);
		keyLocation++;

		if(strcmp(ntlm.getNTLMHash(&currentKey), target) == 0)
		{
			masterThread::setCrackedPassword(currentKey);
			masterThread::setSuccess(true);
		}
		else
		{
			//Increment a local counter for the number of iterations until it reaches a certain point.
			//Once that point has been reached, the local count is committed to the global count and the local
			//variable is reset. This helps keep an accurate count of the iterations without using semaphores.
			if(localProgress > 25000)
			{
				masterThread::increaseIterations(localProgress);
				localProgress = 0;
			}
			else
			{
				localProgress++;
			}
		}

	} while(!masterThread::getSuccess() && keyLocation < endKeyspace);
}