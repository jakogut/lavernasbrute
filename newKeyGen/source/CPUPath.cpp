//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	Director::manageWorker(this);
}

void CPUPath::searchKeyspace()
{
	masterThread::setRemainingTargets(getNumTargets());
	keyGenerator keygen(keyspaceBegin, masterThread::getCharset());

	bool multiHash;

	if(getNumTargets() > 1)
	{
		multiHash = true;
	}
	else
	{
		multiHash = false;
		targetIterator = targets.begin();
	}

	while((keyLocation < keyspaceEnd) && !targets.empty())
	{
		// Get the next key
		currentKey = keygen++;

		keyLocation++;

		// If attacking multiple targets, use the hash map. Otherwise, disable it.
		if(multiHash)
		{
			// Look through the targets for our hash
			targetIterator = targets.find(md4.getWeakHash_NTLM(currentKey));

			if(targetIterator != targets.end()) // Match was found
			{
				masterThread::printResult(targetIterator->second, currentKey);

				removeTarget(targetIterator);
				masterThread::setRemainingTargets(getNumTargets());
			}
		}
		else
		{
			if(md4.getWeakHash_NTLM(currentKey) == targetIterator->first)
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
	// If not, have the Director look for more work. If the director finds work for the thread, we restart the search.
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
	// Replace this with 'return keygen.keyToInteger()' later.
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
