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

	currentKey.reserve(maxChars);

	Director::manageWorker(this);
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
	keyGenerator keygen(keyspaceBegin, masterThread::getCharset());

	bool multiHash = false;

	if(getNumTargets() > 1)
	{
		multiHash = true;
	}
	else
	{
		targetIterator = targets.begin();
	}

	while((keyLocation < keyspaceEnd) && !targets.empty())
	{
		// Get the next key
		currentKey = keygen++;

		// If attacking multiple targets, use the hash map. Otherwise, disable it.
		if(multiHash)
		{
			// Look through the targets for our hash
			targetIterator = targets.find(ntlm.getWeakHash(currentKey));

			if(targetIterator != targets.end()) // Match was found
			{
				masterThread::printResult(targetIterator->second, currentKey);

				removeTarget(targetIterator);
				masterThread::setRemainingTargets(getNumTargets());
			}
		}
		else
		{
			if(ntlm.getWeakHash(currentKey) == targetIterator->first)
			{
				masterThread::printResult(targetIterator->second, currentKey);

				removeTarget(targetIterator);
				masterThread::setRemainingTargets(getNumTargets());
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