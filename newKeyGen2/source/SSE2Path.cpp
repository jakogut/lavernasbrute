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

	for(int i = 0; i < 12; i++)
		currentKeys[i].reserve(maxChars);

	Director::manageWorker(this);
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
	masterThread::setRemainingTargets(getNumTargets());
	NTLM_SSE2 ntlm_md;

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
		keygen.getMultipleKeys(currentKeys, 8);
		keyLocation += 8;

		ntlm_md.getMultipleWeakHashes(currentKeys, weakHashedKeys);

		for(int i = 0; i < 8; i++)
		{
			if(multiHash)
			{
				targetIterator = targets.find(weakHashedKeys[i]);

				if(targetIterator != targets.end()) // Match was found
				{
					masterThread::printResult(targetIterator->second, currentKeys[i]);

					removeTarget(targetIterator);
					masterThread::setRemainingTargets(getNumTargets());
				}
			}
			else
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