//Part of Laverna's Brute

#include "GPUPath.h"

GPUPath::GPUPath(int id)
: id(id)
{
	mRand = new MTRand();
	mNTLM = new NTLM();

	//Multiplying the time by the thread id ensures that the RNG is seeded uniquely for ever thread of every processing path
	mRand->seed((int)(time(NULL) * id));

	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	//Size of data chunks to be processed by the GPU
	chunkSize = 8092;
}

GPUPath::~GPUPath()
{
}

void GPUPath::operator()()
{
	std::string Target = getTarget();

/*	do
	{
		for(int i = 0; i < chunkSize; ++i)
		{
			chunkStringArray[i] = generateRandString(4);
		}

		mNTLM->getMultipleNTLMHashes(chunkSize, chunkStringArray, chunkStringHashedArray);

		for(int i = 0; i < chunkSize; ++i)
		{
			if(chunkStringHashedArray[i] == Target)
			{
				masterThread::setCrackedPassword(chunkStringArray[i]);
				masterThread::setSuccess(true);
				break;
			}
			else
			{
				masterThread::incrementIterations();
			}
		}

	} while(!masterThread::getSuccess());*/
}