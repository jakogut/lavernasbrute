#include "MasterThread.h"
#include "Threads.h"

void masterThread::operator()()
{
	while (getSuccess() == false)
	{
		if(getSuccess() == false)
		{
		}
		else
		{
			break;
		}
	}

	printResult();
}

bool masterThread::getSuccess()
{
	boost::recursive_mutex::scoped_lock scoped_lock(SuccessMutex);
	return success;
}

void masterThread::setSuccess(bool boolean)
{
	boost::recursive_mutex::scoped_lock scoped_lock(SuccessMutex);
	success = boolean;
}

void masterThread::writeIterations()
{
	boost::timed_mutex::scoped_lock scoped_timed_lock(printMutex);
	cout << threads::getIterations() << " iterations" << endl;
}

void masterThread::printResult()
{
	boost::timed_mutex::scoped_lock scoped_lock(printMutex);
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