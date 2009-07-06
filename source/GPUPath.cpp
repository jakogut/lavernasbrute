//Part of Laverna's Brute

#include "GPUPath.h"

GPUPath::GPUPath(int id)
: id(id)
{
	mNTLM = new NTLM();

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
		for(int i = 0; i < chunkSize; i++)
		{
			chunkStringArray[i] = generateRandString(4);
		}

		mNTLM->getMultipleNTLMHashes(chunkSize, chunkStringArray, chunkStringHashedArray);

		for(int i = 0; i < chunkSize; i++)
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