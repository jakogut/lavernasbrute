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

	// Tell the director to manage this path 
	// (If the path finishes its work and becomes idle, the director will find new work for it.)
	Director::manageWorker(this);

	// Initialize the local progress counter to zero
	localProgress = 0;

	currentKey.reserve(maxChars);
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
	masterThread::setRemainingTargets(getNumTargets());
	keyGenerator keygen(keyspaceBegin);

	while((keyLocation < keyspaceEnd) && !targets.empty())
	{
		currentKey = keygen++;

		// NTLM hash the current key, then hash the NTLM hash of the current key, and search the hash map for it. 
		targetIterator = targets.find(ntlm.getWeakHash(currentKey));

		if(targetIterator != targets.end()) // Match was found
		{
			masterThread::printResult(targetIterator->second, currentKey);

			removeTarget(targetIterator);
			masterThread::setRemainingTargets(getNumTargets());
		}
		else // No match
		{
			/* Increment a local counter for the number of iterations until it reaches a certain point.
			Once that point has been reached, the local count is committed to the global count and the local
			variable is reset. This helps keep an accurate count of the iterations without using semaphores. */
			if(localProgress > 1000000)
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
		masterThread::setSuccess();
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