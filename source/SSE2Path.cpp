#include "SSE2Path.h"

SSE2Path::SSE2Path(int id)
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
	for(int i = 0; i < 4; i++)
		currentKeys[i].reserve(maxChars);

	// Tell the director to manage this path 
	// (If the path finishes its work and becomes idle, the director will find new work for it.)
	Director::manageWorker(this);

	// Initialize the local progress counter to zero
	localProgress = 0;
}

SSE2Path::~SSE2Path()
{
}

void SSE2Path::operator()()
{
	searchKeyspace();
}

void SSE2Path::searchKeyspace()
{
	int totalTargets = getNumTargets();
	NTLM_SSE2 ntlm_md;

	while((keyLocation < keyspaceEnd) && !targets.empty())
	{
		// Convert the keyspace location to a key
		integerToKey();

		ntlm_md.getMultipleWeakHashes(currentKeys, weakHashedKeys);

		// NTLM hash the current key, then hash the NTLM hash of the current key, and search the hash map for it.
		for(int i = 0; i < 4; i++)
		{
			targetIterator = targets.find(weakHashedKeys[i]);

			if(targetIterator != targets.end()) // Match was found
			{
				std::cout << "\nHash " << (totalTargets - targets.size()) + 1 << " cracked!" << std::endl
						  << targetIterator->second << " == " << currentKeys[i] << "\n\n";

				removeTarget(targetIterator);
			}
			else // No match
			{
				/* Increment a local counter for the number of iterations until it reaches a certain point.
				Once that point has been reached, the local count is committed to the global count and the local
				variable is reset. This helps keep an accurate count of the iterations without using semaphores. */
				if(localProgress > 250000)
				{
					masterThread::increaseIterations(localProgress);
					localProgress = 0;
				}
			}
		}

		localProgress += 4;
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

int SSE2Path::getThreadID()
{
	return id;
}

void SSE2Path::integerToKey()
{
	for(int i = 0; i < 4; i++)
	{
		currentKeys[i].clear();
		unsigned long long location = keyLocation + i;

		do
		{
			currentKeys[i].append(integerToKeyLookup[location % lookupSize]);
		} while(location /= lookupSize);
	}

	keyLocation += 4;
}

unsigned long long SSE2Path::getKeyspaceEnd()
{
	return keyspaceEnd;
}

unsigned long long SSE2Path::getKeyspaceBegin()
{
	return keyspaceBegin;
}

unsigned long long SSE2Path::getKeyLocation()
{
	return keyLocation;
}

void SSE2Path::moveKeyspaceEnd(unsigned long long input)
{
	keyspaceEnd = input;
}

void SSE2Path::moveKeyspaceBegin(unsigned long long input)
{
	keyspaceBegin = input;
}

void SSE2Path::moveKeylocation(unsigned long long input)
{
	keyLocation = input;
}