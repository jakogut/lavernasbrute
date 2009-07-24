//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	unsigned long long keyspaceSize = (unsigned long long)(pow((double)charsetLength, maxChars) / totalThreads);

	startKeyspace = keyspaceSize  * id;
	endKeyspace = startKeyspace + keyspaceSize;

	//Assign a unique portion of the keyspace to the thread
	keyLocation = startKeyspace;
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	localProgress = 0;

	do
	{
		char* currentKey = new char[maxChars];

		integerToKey(keyLocation, currentKey);
		keyLocation++;

		//Compare strings and take tha appropriate action if the hashes are equal
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
			if(localProgress > 250000)
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
void CPUPath::integerToKey(unsigned long long location, char* output)
{
	unsigned long long num = location;
	int length = charsetLength + 1;

	for(int i = 0; num > 0; i++)
	{
		output[i] = charset[num % length];
		num /= length;
	}
}