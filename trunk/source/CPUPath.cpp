//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	//Assign a unique portion of the keyspace to the thread (Based on id)
	keyspaceSize = (unsigned long long)(pow((long double)charsetLength, maxChars) / totalThreads);

	startKeyspace = (keyspaceSize * id);
	endKeyspace = (startKeyspace + keyspaceSize);

	keyLocation = startKeyspace;

	localProgress = 0;
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	masterThread::setNumTargets(getNumTargets());
	int totalTargets = getNumTargets();

	while(!masterThread::getSuccess() && (keyLocation < endKeyspace) && targets.size() > 0)
	{
		//Convert the keyspace location to a key
		integerToKey(keyLocation);
		keyLocation++;

		//Hash and compare
		std::string hashedKey = ntlm.getNTLMHash(&currentKey);

		for(long i = 0; i < targets.size(); i++)
		{
			if(hashedKey == targets[i]) //Comparison returns a match
			{
				masterThread::pushTargetHash(targets[i]);
				masterThread::pushCrackedHash(currentKey);

				std::cout << "Hash " << (totalTargets - targets.size()) + 1 << " cracked!" << std::endl;

				targets.erase(targets.begin() + i);
			}
			else //No match
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
		}
	}

	masterThread::setSuccess(true);
}

//Convert the integer key location to a text string using base conversion
void CPUPath::integerToKey(unsigned long long location)
{
	unsigned long long num = location;
	long long lookupSize = masterThread::getLookupSize();
	long i = 0;

	currentKey.clear();

	for(; num > 0; i++)
	{
		currentKey += integerToKeyLookup[num % lookupSize];
		num /= lookupSize;
	}
}