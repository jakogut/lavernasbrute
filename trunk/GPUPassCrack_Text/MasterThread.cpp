//Part of Laverna's Brute

#include "MasterThread.h"
#include "Threads.h"

masterThread::masterThread(int id, time_t time) 
: id(id), startTime(time)
{
}

masterThread::~masterThread()
{
}

void masterThread::operator()()
{
	while (getSuccess() == false)
	{
	}

	printResult();
}

bool masterThread::getSuccess()
{
	boost::mutex::scoped_lock scoped_lock(SuccessMutex);
	return success;
}

void masterThread::setSuccess(bool boolean)
{
	boost::mutex::scoped_lock scoped_lock(SuccessMutex);
	success = boolean;
}

void masterThread::writeIterations()
{
	boost::mutex::scoped_lock scoped_lock(printMutex);
	cout << threads::getIterations() << " iterations" << endl;
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

	cout << endl << "Successful Attack! Attack duration: " << threads::getIterations() << " iterations." << endl
		<< "Completed in: " << hours << " hours, " << minutes << " minutes, and " << seconds
		<< " seconds. " << endl;
			
	if(endTime - startTime != 0)
	{
		cout << "Speed of this run: " << (threads::getIterations() / (endTime - startTime)) << " Iterations per second." << endl;
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