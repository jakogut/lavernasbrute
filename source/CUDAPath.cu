// Part of Laverna's Brute

#include "CUDAPath.h"

#include "messageGenerators/messageGenerator_NTLM.h"

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
	ctx = new hashContext[bufferSize];

	for(int i = 0; i < bufferSize; i++) messgen.integerToMessage(&ctx[i], keyLocation + i);

	while((keyLocation < keyspaceEnd) && (masterThread::getSuccess() == false))
	{
		cu_md4.getHashContext(ctx, bufferSize);

		for(int i = 0; i < bufferSize; i++)
		{
			if(bloomCheck(bFilter, &ctx[i].wd[0]))
			{
				targetIterator = binarySearch(targets.begin(), target.end(), &ctx);

				if(targetIterator != target.end())
				{
					masterThread::printResult(md4.contextToHash(&ctx[i]), messgen.messageToKey(&ctx));
					masterThread::setRemainingTarget(masterThread::getRemainingTargets() - 1);
				}
			}

			messgen.stepMessage(&ctx[i]);
			++keyLocation;
		}
	}

	if((masterThread::getRemainingTargets() > 0) && masterThread::reassignKeyspace(this))
		searchKeyspace();
}
