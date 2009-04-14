//Part of Laverna's Brute

#include "MasterThread.h"
#include "CPUPath.h"
#include "GPUPath.h"


masterThread::masterThread(int id, time_t time) 
: id(id), startTime(time)
{
}

masterThread::~masterThread()
{
}

void masterThread::operator()()
{
	do
	{
		if(!getSilent())
		{
			if(getIterations() % getInterval() == 0)
			{
				writeIterations();
			}
		}

	} while(!getSuccess());

	printResult();
}

bool masterThread::getSuccess()
{
	boost::mutex::scoped_lock lock(SuccessMutex);
	return success;
}

void masterThread::setSuccess(bool boolean)
{
	//boost::mutex::scoped_lock scoped_lock(SuccessMutex);
	success = boolean;
}

char* masterThread::getCharset()
{
	return randCharset;
}

int masterThread::getCharsetLength()
{
	return charsetLength;
}

void masterThread::incrementIterations()
{
	++iterations;
}

void masterThread::increaseIterations(int input)
{
	iterations += input;
}

long long masterThread::getIterations()
{
	return iterations;
}

void masterThread::writeIterations()
{
	boost::try_mutex::scoped_lock scoped_lock(printMutex);
	cout << getIterations() << " iterations" << endl;
}

void masterThread::printResult()
{
	boost::mutex::scoped_lock scoped_lock(printMutex);
	time_t endTime = ((unsigned)time(NULL));

	time_t hours = ((endTime - startTime) / 3600);

	if(hours < 1)
		hours = 0;

	time_t minutes = ((endTime - startTime) / 60) - (hours * 60);
	time_t seconds = (endTime - startTime) - (minutes * 60);

	cout << endl << "Successful Attack! Cracked password is: " << crackedPassword << endl
		<< "Attack duration: " << getIterations() << " iterations." << endl
		<< "Completed in: " << hours << " hours, " << minutes << " minutes, and " << seconds
		<< " seconds. " << endl;
			
	if(endTime - startTime != 0)
	{
		cout << "Speed of this run: " << (getIterations() / (endTime - startTime)) << " Iterations per second." << endl;
	}
}

//No mutexes needed, these are accessed infrequently.
bool masterThread::getSilent()
{
	return silent;
}

void masterThread::setSilent(bool write)
{
	silent = write;
}

int masterThread::getInterval()
{
	return interval;
}

void masterThread::setInterval(int write)
{
	interval = write;
}

void masterThread::setCrackedPassword(string write)
{
	crackedPassword = write;
}