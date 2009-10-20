//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	// Assign a unique portion of the keyspace to the thread (Based on id)
	keyspaceSize = (masterThread::pow(charsetLength, maxChars) / totalThreads);

	keyspaceBegin = (keyspaceSize * id);
	keyspaceEnd = (keyspaceBegin + keyspaceSize);

    // Set the key location
	keyLocation = keyspaceBegin;

    // Get the integer to key lookup size
	lookupSize = masterThread::getLookupSize();
    
    // Reserve space in our strings
	currentKey.reserve(maxChars);
	hashedKey.reserve(32);

	Director::setWorkerPtr(this, getThreadID());
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	searchKeyspace();
}

void CPUPath::searchKeyspace()
{
	int totalTargets = getNumTargets();

	std::map<std::string, std::string>::iterator targetIterator;

	while((keyLocation < keyspaceEnd) && !targets.empty())
	{
		currentKey.clear();

		// Convert the keyspace location to a key
		unsigned long long convert = keyLocation;
		integerToKey(convert);

		// Hash and compare 
		hashedKey = ntlm.getNTLMHash((char*)currentKey.c_str());

		targetIterator = targets.find(hashedKey.substr(0, 5));

		if((targetIterator != targets.end()) && (targetIterator->second == hashedKey)) // Match was found
		{
			std::cout << "\nHash " << (totalTargets - targets.size()) + 1 << " cracked!" << std::endl
					  << targetIterator->second << " == " << currentKey << "\n\n";

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
	else
	{
		Director::reassignKeyspace(this);
		searchKeyspace();
	}
}

int CPUPath::getThreadID()
{
	return id;
}

// Convert the integer key location to a text string using base conversion
void CPUPath::integerToKey(unsigned long long location)
{
	if(location)
	{
		currentKey += integerToKeyLookup[location % lookupSize];
		location /= lookupSize;

		integerToKey(location);
	}
	else
		keyLocation++;
}

unsigned long long CPUPath::getKeyspaceEnd()
{
	return keyspaceEnd;
}

unsigned long long CPUPath::getKeyspaceBegin()
{
	return keyspaceBegin;
}

unsigned long long CPUPath::getKeyLocation()
{
	return keyLocation;
}

void CPUPath::moveKeyspaceEnd(unsigned long long input)
{
	keyspaceEnd = input;
}

void CPUPath::moveKeyspaceBegin(unsigned long long input)
{
	keyspaceBegin = input;
}

void CPUPath::moveKeylocation(unsigned long long input)
{
	keyLocation = input;
}