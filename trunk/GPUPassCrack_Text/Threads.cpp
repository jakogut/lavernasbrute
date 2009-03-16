//Part of Laverna's Brute

#include "Threads.h"
#include "MasterThread.h"

threads::threads(int id) 
: id(id)
{
	mRand = new MTRand();
	mRand->seed(id);
}

threads::~threads()
{
}

void threads::operator()()
{
	string Passwd = getPasswd();

	while (!masterThread::getSuccess())
	{
		setPasswdNumericalString(generateRandString(Passwd.length()));

		if(passwdNumericalString == Passwd)
		{
			masterThread::setSuccess(true);
			break;
		}
		else
		{
			incrementIterations();

			if(!masterThread::getSilent())
			{
				if(getIterations() % masterThread::getInterval() == 0)
				{
					masterThread::writeIterations();
				}
			}
		}
	}
}

void threads::incrementIterations()
{
	++iterations;
}

__int64 threads::getIterations()
{
	return iterations;
}

string threads::getPasswdNumericalString()
{
	return passwdNumericalString;
}

void threads::setPasswdNumericalString(string write)
{
	passwdNumericalString = write;
}

string threads::getPasswd()
{
	return passwd;
}

void threads::writePasswd(string write)
{
	passwd = write;
}

void threads::setRandFast(bool write)
{
	randFast = write;
}

//Generate random string of numbers to be converted to text. Who says randomness isn't useful?
string threads::generateRandString(int length)
{
   string result = "";

	if(randFast)
	{
		for(int i = 0; i < length; ++i) 
		{
			result += RAND_CHARSET[rand() % CHARSET_LENGTH];
		}
	}
	else
	{
		for(int i = 0; i < length; ++i) 
		{
			result += RAND_CHARSET[mRand->randInt(CHARSET_LENGTH)];
		}
   }

   return result;
}