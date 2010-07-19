// Part of Laverna's Brute

#include "SSEPath.h"

#ifdef SSE

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
	MD4_SSE md4;

	keyGenerator keygen(keyspaceBegin, masterThread::getCharset());

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
		md4.getHashContext_NTLM(currentKeys, currentContexts);

		// Check our target hash map for matches
		for(int i = 0; i < 12; i++)
		{
			if(bloomCheck(bFilter, currentContexts[i].uint32[0]))
			{
				targetIterator = binarySearch(targets.begin(), targets.end(), &currentContexts[i]);

				if(targetIterator != targets.end())
				{
					masterThread::printResult("placeholder", currentKeys[i]);
					// Reduce the target number here
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

#endif
