#include "GPUPath.h"

GPUPath::GPUPath(int id)
: id(id)
{
	//We give the GPU thread four times the keyspace size than a normal CPU thread
	keyspaceSize = (unsigned long long)(pow((long double)charsetLength, maxChars) / totalThreads);

	startKeyspace = (keyspaceSize * id);
	endKeyspace = (startKeyspace + keyspaceSize);

	keyLocation = startKeyspace;

	chunkSize = 1500000;

	localProgress = 0;
}

GPUPath::~GPUPath()
{
}

void GPUPath::operator ()()
{
	ntlm = new NTLM_GPU(chunkSize);

	while(!masterThread::getSuccess() && (keyLocation < endKeyspace))
	{
		keyBuffer = new std::string[chunkSize];
		hashBuffer = new std::string[chunkSize];

		integerToKey();

		ntlm->getMultipleNTLMHashes(keyBuffer, hashBuffer);

		for(int i = 0; i < chunkSize; i++)
		{
			if(strcmp((char*)hashBuffer[i].c_str(), target) == 0)
			{
				masterThread::setCrackedPassword(keyBuffer[i]);
				masterThread::setSuccess(true);
			}
		}

		masterThread::increaseIterations(chunkSize);

		delete [] keyBuffer;
		delete [] hashBuffer;
	}

	delete ntlm;
}

//Might want to look into making this GPU accelerated
void GPUPath::integerToKey()
{
	int lookupSize = masterThread::getLookupSize();

	for(int i = 0; i < chunkSize; i++)
	{
		unsigned long long num = keyLocation;

		while(num > 0)
		{
			keyBuffer[i] += integerToKeyLookup[num % lookupSize];
			num /= lookupSize;
		}

		keyLocation++;
	}
}