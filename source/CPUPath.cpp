//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	keyspaceSize = (unsigned long long)(pow((long double)charsetLength, maxChars) / totalThreads);

	startKeyspace = (keyspaceSize  * id);
	endKeyspace = (startKeyspace + keyspaceSize);

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
		currentKey = new char[maxChars + 1];

		integerToKey(keyLocation);
		keyLocation++;

		if(strcmp(ntlm.getNTLMHash(currentKey), target) == 0)
		{
			masterThread::setCrackedPassword(currentKey);
			masterThread::setSuccess(true);
		}
		else
		{
			//Increment a local counter for the number of iterations until it reaches a certain point.
			//Once that point has been reached, the local count is committed to the global count and the local
			//variable is reset. This helps keep an accurate count of the iterations without using semaphores.
			if(localProgress > 100000)
			{
				masterThread::increaseIterations(localProgress);
				localProgress = 0;
			}
			else
			{
				localProgress++;
			}
		}

		delete [] currentKey;

	} while(!masterThread::getSuccess() && keyLocation < endKeyspace);
}

//Convert the integer key location to a text string
void CPUPath::integerToKey(unsigned long long location)
{
	unsigned long long num = location;
	int i = 0;

	for(; num > 0; i++)
	{
		currentKey[i] = charset[num % (charsetLength + 1)];
		num /= charsetLength + 1;
	}

	currentKey[i + 1] = 0;
}