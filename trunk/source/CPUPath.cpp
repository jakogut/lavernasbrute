//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	// Assign a unique portion of the keyspace to the thread (Based on id)
	keyspaceSize = (masterThread::pow(charsetLength, maxChars) / numWorkers);

	keyspaceBegin = (keyspaceSize * id);
	keyspaceEnd = (keyspaceBegin + keyspaceSize);

    // Set the key location
	keyLocation = keyspaceBegin;

    // Get the integer to key lookup size
	lookupSize = masterThread::getLookupSize();
    
    // Reserve space in our strings
	currentKey.reserve(maxChars);

	// Tell the director to manage this path 
	// (If the path finishes its work and becomes idle, the director will find new work for it.)
	Director::manageWorker(this);

	// Initialize the local progress counter to zero
	localProgress = 0;
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

	while((keyLocation < keyspaceEnd) && !targets.empty())
	{
		currentKey.clear();

		// Convert the keyspace location to a key
		unsigned long long convert = keyLocation;
		integerToKey(convert);

		// NTLM hash the current key, then hash the NTLM hash of the current key, and search the hash map for it. 
		targetIterator = targets.find(hash(ntlm.getNTLMHash(&currentKey)));

		if(targetIterator != targets.end()) // Match was found
		{
			std::cout << "\nHash " << (totalTargets - targets.size()) + 1 << " cracked!" << std::endl
					  << targetIterator->second << " == " << currentKey << "\n\n";

			removeTarget(targetIterator);
		}
		else // No match
		{
			/* Increment a local counter for the number of iterations until it reaches a certain point.
			Once that point has been reached, the local count is committed to the global count and the local
			variable is reset. This helps keep an accurate count of the iterations without using semaphores. */
			if(localProgress > 500000)
			{
				masterThread::increaseIterations(localProgress);
				localProgress = 0;
			}
			else
			{
				// Loop is done, increment the local iteration counter
				localProgress++;
			}
		}
	}

	// If all targets have been cracked, rejoice and signal the master thread that we're done.
	if(targets.empty())
	{
		masterThread::setSuccess(true);
	}
	// If not, ask the director if we can have more work. If the director finds work for us, we restart the search.
	else if(Director::reassignKeyspace(this))
	{
		searchKeyspace();
	}
}

int CPUPath::getThreadID()
{
	return id;
}

// Convert the integer key location to a text string using recursive base conversion.
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