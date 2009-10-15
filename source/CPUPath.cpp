//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	//Assign a unique portion of the keyspace to the thread (Based on id)
	keyspaceSize = (masterThread::pow(charsetLength, maxChars) / totalThreads);

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

	std::map<std::string, std::string>::iterator targetIterator;

	while(!masterThread::getSuccess() && (keyLocation < endKeyspace) && !targets.empty())
	{
		// Convert the keyspace location to a key
		integerToKey(keyLocation);
		keyLocation++;

		// Hash and compare 
		hashedKey = ntlm.getNTLMHash((char*)currentKey.c_str());

		targetIterator = targets.find(hashedKey.substr(0, 5));

		if((targetIterator != targets.end()) && (targetIterator->second == hashedKey)) // Match was found
		{
			masterThread::pushTargetHash(targetIterator->second);
			masterThread::pushCrackedHash(currentKey);

			std::cout << "Hash " << (totalTargets - targets.size()) + 1 << " cracked!" << std::endl;

			targets.erase(targetIterator);
		}
		else // No match
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
				//Loop is done, increment the local iteration counter
				localProgress++;
			}
		}
	}

	if(targets.empty())
		masterThread::setSuccess(true);
}

//Convert the integer key location to a text string using base conversion
void CPUPath::integerToKey(unsigned long location)
{
	unsigned long num = location;
	int lookupSize = masterThread::getLookupSize();

	currentKey.clear();

	while(num > 0)
	{
		currentKey += integerToKeyLookup[num % lookupSize];
		num /= lookupSize;
	}
}