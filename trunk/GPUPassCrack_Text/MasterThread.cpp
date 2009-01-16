#include "MasterThread.h"
#include "Threads.h"

void masterThread::operator()()
{
	while (getSuccess() == false)
	{
		if(getSuccess() == false)
		{
			if(threads::getIterations() % 250000 == 0)
			{
				writeIterations();
			}							
		}
		else
		{
			time_t endTime = ((unsigned)time(NULL));

			time_t hours = ((endTime - startTime) / 3600);

			if(hours < 1)
				hours = 0;

			time_t minutes = ((endTime - startTime) / 60) - (hours * 60);
			time_t seconds = (endTime - startTime) - (minutes * 60);

			boost::recursive_mutex::scoped_lock scoped_lock(threads::readIterationsMutex);
			cout << endl << "Successful Attack! Attack duration: " << threads::getIterations() << " iterations." << endl
				<< "Completed in: " << hours << " hours, " << minutes << " minutes, and " << seconds
				<< " seconds. " << endl;
			
			if(endTime - startTime != 0)
			{
				cout << "Speed of this run: " << (threads::getIterations() / (endTime - startTime)) << " Iterations per second." << endl;
			}

			break;
		}
	}
}

bool masterThread::getSuccess()
{
	boost::recursive_mutex::scoped_lock scoped_lock(readSuccessMutex);
	return success;
}

void masterThread::setSuccess(bool boolean)
{
	boost::recursive_mutex::scoped_lock scoped_lock(writeSuccessMutex);
	success = boolean;
}

void masterThread::writeIterations()
{
	boost::recursive_mutex::scoped_lock scoped_lock(threads::readIterationsMutex);
	cout << threads::getIterations() << " iterations" << endl;
}