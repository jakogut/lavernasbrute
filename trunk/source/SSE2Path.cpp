// Part of Laverna's Brute

#include "SSE2Path.h"

SSE2Path::SSE2Path(int id)
: id(id)
{
}

SSE2Path::~SSE2Path()
{
}

void SSE2Path::operator()()
{
	Director::manageWorker(this);
}

void SSE2Path::searchKeyspace()
{
	masterThread::setRemainingTargets(getNumTargets());
	MD4_SSE2 md4_sse2;

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

	for(int i = 0; i < 12; i++) currentKeys[i] = new char[16];

	while((keyLocation < keyspaceEnd) && !targets.empty())
	{
		// Twelve keys are processed per round in this path
		for(int i = 0; i < 12; i++, keyLocation++)
		{
			memcpy(currentKeys[i], keygen.getKey(), 16);
			keygen.incrementKey();
		}

		// Hash the keys
		md4_sse2.getWeakHashes_NTLM(currentKeys, weakHashedKeys);

		// Check our target hash map for matches
		for(int i = 0; i < 12; i++)
		{
			if(multiHash)
			{
				targetIterator = targets.find(weakHashedKeys[i]);

				if(targetIterator != targets.end())
				{
					masterThread::printResult(targetIterator->second, currentKeys[i]);

					removeTarget(targetIterator);
					masterThread::setRemainingTargets(getNumTargets());
				}
			}
			else // If there's only a single hash, disable searching the hash map to improve speed.
			{
				if(weakHashedKeys[i] == targetIterator->first)
				{
					masterThread::printResult(targetIterator->second, currentKeys[i]);

					removeTarget(targetIterator);
					masterThread::setRemainingTargets(getNumTargets());
				}
			}
		}
	}

	if(targets.empty())
	{
		masterThread::setSuccess();
	}
	else if(Director::reassignKeyspace(this))
	{
		searchKeyspace();
	}
}

int SSE2Path::getThreadID()
{
	return id;
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
