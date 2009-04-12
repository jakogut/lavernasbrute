//Part of Laverna's Brute

#include "GPUPath.h"

GPUPath::GPUPath(int id)
: id(id)
{
	mRand = new MTRand();

	//Multiplying the time by the thread id ensures that the RNG is seeded uniquely for ever thread of every processing path
	mRand->seed((int)(time(NULL) * id));

	mNTLM = new NTLM();

	randCharset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	//Size of data chunks to be processed by the GPU
	chunkSize = 10;
}

GPUPath::~GPUPath()
{
}

void GPUPath::operator()()
{
	//Arrays to move data chunks
	string* chunkStringArray = new string[chunkSize];
	string* chunkStringHashedArray = new string[chunkSize];

	string Passwd = getPasswd();

	do
	{
		for(int i = 0; i < chunkSize; ++i)
		{
			chunkStringArray[i] = generateRandString(4);
		}

		mNTLM->getMultipleNTLMHashes(chunkSize, chunkStringArray, chunkStringHashedArray);

		for(int i = 0; i < chunkSize; ++i)
		{
			if(chunkStringHashedArray[i] == Passwd)
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

	} while(!masterThread::getSuccess());

	delete [] chunkStringArray;
	delete [] chunkStringHashedArray;
}