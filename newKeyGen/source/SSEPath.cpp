// Part of Laverna's Brute

#include "SSEPath.h"

SSEPath::SSEPath(int id)
: id(id)
{
}

SSEPath::~SSEPath()
{
}

void SSEPath::operator()()
{
	Director::manageWorker(this);
}

void SSEPath::searchKeyspace()
{
	masterThread::setRemainingTargets(getNumTargets());
	MD4_SSE md4_SSE;

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
		md4_SSE.getHashContext_NTLM(currentKeys, weakHashedKeys);

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

unsigned int SSEPath::getThreadID()
{
	return id;
}

unsigned long long SSEPath::getKeyspaceEnd()
{
	return keyspaceEnd;
}

unsigned long long SSEPath::getKeyspaceBegin()
{
	return keyspaceBegin;
}

unsigned long long SSEPath::getKeyLocation()
{
	return keyLocation;
}

void SSEPath::moveKeyspaceEnd(unsigned long long input)
{
	keyspaceEnd = input;
}

void SSEPath::moveKeyspaceBegin(unsigned long long input)
{
	keyspaceBegin = input;
}

void SSEPath::moveKeylocation(unsigned long long input)
{
	keyLocation = input;
}