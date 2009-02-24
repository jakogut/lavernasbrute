#include "Threads.h"
#include "MasterThread.h"

void threads::operator()()
{
	while (!masterThread::getSuccess())
	{
		setPasswdNumericalString(generateRandString(getPasswd().length()));

		if(passwdNumericalString == getPasswd())
		{
			masterThread::setSuccess(true);
			break;
		}
		else
		{
			incrementIterations();

			if(!masterThread::getSilent())
			{
				if(threads::getIterations() % masterThread::getInterval() == 0)
				{
					masterThread::writeIterations();
				}
			}
		}
	}
}

void threads::generatePasswdString(string passwd)
{
	setPasswdNumericalString(generateRandString(passwd.length()));
}

//We're only reading the variable here, so there's no need to lock it. It's not imperative that we have
//the variable returned every time either. That's good, because sometimes the variable won't be returned
//as a result of being locked by the incrementIterations function.
__int64 threads::getIterations()
{
	//boost::mutex::scoped_lock scoped_lock(IterationsMutex);
	return iterations;
}

void threads::incrementIterations()
{
	//boost::mutex::scoped_lock lock(IterationsMutex);
	iterations++;
}

string threads::getPasswdNumericalString()
{
	//boost::recursive_mutex::scoped_lock scoped_lock(PasswdNumericalStringMutex);
	return passwdNumericalString;
}

void threads::setPasswdNumericalString(string write)
{
	boost::recursive_mutex::scoped_lock scoped_lock(PasswdNumericalStringMutex);
	passwdNumericalString = write;
}

string threads::getPasswd()
{
	boost::recursive_mutex::scoped_lock scoped_lock(PasswdMutex);
	return passwd;
}

void threads::writePasswd(string write)
{
	boost::recursive_mutex::scoped_lock scoped_lock(PasswdMutex);
	passwd = write;
}

//Generate random string of numbers to be converted to text
string threads::generateRandString(int length)
{
	string passwdNumericalString = "";

	char temp;
	int randTemp;
 
	for(int i = 0; i < length; i++)
	{
		randTemp = random();

		//Discard integer if it goes out of scope of the ASCII character set.
		if(!(randTemp == 32 || (randTemp >= 48 && randTemp <= 57) || (randTemp >= 65 && randTemp <= 90) || (randTemp >= 97 && randTemp <= 122)))
		{
			i--;
			continue;
		}
		else
		{
			temp = randTemp;
			passwdNumericalString += temp;
		}
	}

	return passwdNumericalString;
}