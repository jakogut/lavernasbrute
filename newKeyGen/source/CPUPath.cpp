//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	if(hashType == "ntlm")
		hashFunc = &MD4::getHashContext_NTLM;
	else if(hashType == "md4")
		hashFunc = &MD4::gethashContext;;
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

	hashContext* currentContext;

	while((keyLocation < keyspaceEnd) && !targets.empty())
	{
		// Get the next key
		currentKey = keygen++;
		keyLocation++;

		currentContext = (md4.*hashFunc)(currentKey);

		// Check the bloom filter for our hash first
		if(bloomCheck(bFilter, currentContext->uint32[0]))
		{
			// The bloom filter returned positive, look through the target list for our hash
			targetIterator = binarySearch(targets.begin(), targets.end(), currentContext);
			
			if(targetIterator != targets.end()) // Match was found
			{
				masterThread::printResult("placeholder", currentKey);
				// Reduce the target number here
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

unsigned int CPUPath::getThreadID()
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

