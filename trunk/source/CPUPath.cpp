//Part of Laverna's Brute

#include "CPUPath.h"

CPUPath::CPUPath(int id)
: id(id)
{
	mRand = new MTRand();

	//Multiplying the time by the thread id ensures that the RNG is seeded uniquely for ever thread of every processing path
	mRand->seed((int)(time(NULL) * id));

	mNTLM = new NTLM();

	randCharset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();
}

CPUPath::~CPUPath()
{
}

void CPUPath::operator()()
{
	string Passwd = getPasswd();

	do
	{
		string tempString = "";
		tempString = generateRandString(4);

		if(mNTLM->getNTLMHash(tempString) == Passwd)
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