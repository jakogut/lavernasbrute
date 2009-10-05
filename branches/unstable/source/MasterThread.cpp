//Part of Laverna's Brute

#include "MasterThread.h"

masterThread::masterThread() 
: id(id)
{
	//Start the clock
	startTime = time(NULL);

	charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	charsetLength = (int)(strlen(charset));
}

masterThread::~masterThread()
{
}

void masterThread::operator()()
{
	boost::posix_time::seconds updateInterval(interval);

	if(!silent)
	{
		while(!success)
		{
			writeIterations();
			boost::this_thread::sleep(updateInterval);
		}
	}
	else
	{
		while(!success);
			boost::this_thread::sleep(updateInterval);
	}

	printResult();
}

void masterThread::writeIterations()
{
	std::cout << getIterations() << " iterations" << std::endl;
}

void masterThread::printResult()
{
	time_t endTime = time(NULL);

	time_t seconds = (endTime - startTime);
	time_t minutes = seconds / 60;
	time_t hours = minutes / 60;
	time_t days = hours / 24;

	hours -= days * 24;
	minutes -= hours * 60;
	seconds -= minutes * 60;

	std::cout << std::endl << "Successful Attack! Cracked password is: " << crackedPassword << std::endl
		<< "Attack duration: " << getIterations() << " iterations." << std::endl
		<< "Completed in: " << hours << " hours, " << minutes << " minutes, and " << seconds
		<< " seconds. " << std::endl;
			
	if(endTime - startTime != 0)
	{
		std::cout << "Speed of this run: " << (getIterations() / (endTime - startTime)) << " Iterations per second." << std::endl;
	}
}

bool masterThread::getSuccess()
{
	return success;
}

void masterThread::incrementIterations()
{
	iterations++;
}

void masterThread::increaseIterations(unsigned long long input)
{
	iterations += input;
}

void masterThread::setSuccess(bool boolean)
{
	success = boolean;
}

char* masterThread::getCharset()
{
	return charset;
}

int masterThread::getCharsetLength()
{
	return charsetLength;
}

long long masterThread::getIterations()
{
	return iterations;
}

bool masterThread::getSilent()
{
	return silent;
}

void masterThread::setSilent(bool write)
{
	silent = write;
}

void masterThread::setInterval(int write)
{
	interval = write;
}

void masterThread::setCrackedPassword(std::string write)
{
	crackedPassword = write;
}