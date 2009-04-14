//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	mRand = new MTRand();
	mNTLM = new NTLM();

	//Multiplying the time by the thread id ensures that the RNG is seeded uniquely for ever thread of every processing path
	mRand->seed((int)(time(NULL) * id));

	randCharset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	string Target = getTarget();

	do
	{
		string tempString = "";
		tempString = generateRandString(4);

		if(mNTLM->getNTLMHash(tempString) == Target)
		{
			masterThread::setCrackedPassword(tempString);
			masterThread::setSuccess(true);
			break;
		}
		else
		{
			masterThread::incrementIterations();
		}

	} while(!masterThread::getSuccess());
}