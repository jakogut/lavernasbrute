//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	mNTLM = new NTLM();

	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	startKeyspace = (long long)((pow((double)charsetLength, maxChars) / totalThreads) * id);
	endKeyspace = startKeyspace + (long long)(pow((double)charsetLength, maxChars) / totalThreads);

	//Assign a unique portion of the keyspace to the thread
	keyLocation = startKeyspace;
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	std::string tempString;

	do
	{
		tempString = integerToKey(&keyLocation);
		keyLocation++;

		if(mNTLM->getNTLMHash(tempString) == target)
		{
			masterThread::setCrackedPassword(tempString);
			masterThread::setSuccess(true);
			break;
		}
		else
		{
			masterThread::incrementIterations();
		}

	} while(!masterThread::getSuccess() && keyLocation < endKeyspace);
}