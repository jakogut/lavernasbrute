// Part of Laverna's Brute

#include "CUDAPath.h"

#include "messageGenerators/MessageGenerator_NTLM.h"

#define threadsPerBlock 128
#define blocksPerGrid 1024
#define threadsPerGrid (threadsPerBlock * blocksPerGrid)

// Later we can make it so that the buffer stores multiple grids worth of contexts
#define bufferSize (threadsPerGrid * 1)

CUDAPath::CUDAPath(int id)
	: id(id)
{
}

CUDAPath::~CUDAPath()
{
}

void CUDAPath::operator()()
{
	masterThread::manageWorker(this);
}

void CUDAPath::searchKeyspace()
{
	messageGenerator_NTLM messgen(masterThread::getCharset());

	cu_md4 = new MD4_CUDA(threadsPerBlock, blocksPerGrid);
	ctx = new hashContext[bufferSize];

	for(int i = 0; i < bufferSize; i++) messgen.integerToMessage(&ctx[i], keyLocation + i);

	while((keyLocation < keyspaceEnd) && (masterThread::getSuccess() == false))
	{
		cu_md4->getHashContext(ctx, bufferSize);

		for(int i = 0; i < bufferSize; i++)
		{
			if(bloomCheck(bFilter, &ctx[i].wd[0]))
			{
				targetIterator = binarySearch(targets.begin(), targets.end(), &ctx[i]);

				if(targetIterator != targets.end())
				{
					masterThread::printResult(md4.contextToHash(&ctx[i]), messgen.messageToKey(&ctx[i]));
					masterThread::setRemainingTargets(masterThread::getRemainingTargets() - 1);
				}
			}
		}

		for(int i = 0; i < bufferSize; i++) messgen.stepMessage(&ctx[i], bufferSize);
		keyLocation += bufferSize;
	}

	delete [] ctx;
	delete cu_md4;

	if((masterThread::getRemainingTargets() > 0) && masterThread::reassignKeyspace(this))
		searchKeyspace();
}

unsigned int CUDAPath::getThreadID()
{
	return id;
}

unsigned long long CUDAPath::getKeyspaceEnd()
{
	return keyspaceEnd;
}

unsigned long long CUDAPath::getKeyspaceBegin()
{
	return keyspaceBegin;
}

unsigned long long CUDAPath::getKeyLocation()
{
	return keyLocation;
}

void CUDAPath::moveKeyspaceEnd(unsigned long long input)
{
	keyspaceEnd = input;
}

void CUDAPath::moveKeyspaceBegin(unsigned long long input)
{
	keyspaceBegin = input;
}

void CUDAPath::moveKeylocation(unsigned long long input)
{
	keyLocation = input;
}


