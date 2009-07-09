//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	startKeyspace = ((pow((double)charsetLength, maxChars) / totalThreads) * id);
	endKeyspace = startKeyspace + (pow((double)charsetLength, maxChars) / totalThreads);

	//Assign a unique portion of the keyspace to the thread
	keyLocation = startKeyspace;

	std::cout << id << "\t" << startKeyspace << " - " << endKeyspace << std::endl;
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	do
	{
		integerToKey(&keyLocation, &currentKey);
		keyLocation++;

		if(ntlm.getNTLMHash(currentKey) == target)
		{
			masterThread::setCrackedPassword(currentKey);
			masterThread::setSuccess(true);
		}
		else
		{
			masterThread::incrementIterations();
		}

	} while(!masterThread::getSuccess() && keyLocation < endKeyspace);
}