//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	//Assign a unique portion of the keyspace to the thread (Based on id)
	keyspaceSize = (pow(charsetLength, maxChars) / totalThreads);

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

	currentKey = new char[maxChars + 1];
	hashedKey = new char[33];

	while(!masterThread::getSuccess() && (keyLocation < endKeyspace) && (targets.size() > 0))
	{
		// Convert the keyspace location to a key
		integerToKey(keyLocation);
		keyLocation++;

		// Hash and compare 
		hashedKey = ntlm.getNTLMHash(currentKey);

		int numTargets = getNumTargets();
		for(long i = 0; i < numTargets; i++)
		{
			if(strcmp(hashedKey, (char*)targets[i].c_str()) == 0) //Comparison returns a match
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
			}
		}

		//Loop is done, increment the local iteration counter
		localProgress++;
	}

	masterThread::setSuccess(true);

	delete [] currentKey;
	delete [] hashedKey;
}

//Convert the integer key location to a text string using base conversion
void CPUPath::integerToKey(unsigned long location)
{
	unsigned long num = location;
	int lookupSize = masterThread::getLookupSize();

	int i = 0;
	std::string key;

	while(num > 0)
	{
		char* append = integerToKeyLookup[num % lookupSize];
		int appendLength = strlen(append);

		for(int j = 0; j < appendLength; j++)
		{
			currentKey[i++] = append[j];
		}

		num /= lookupSize;
	}

	currentKey[i] = '\0';
}