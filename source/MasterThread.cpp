//Part of Laverna's Brute

#include "MasterThread.h"

masterThread::masterThread(int id) 
: id(id)
{
	//Start the clock
	startTime = time(NULL);

	charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	charsetLength = strlen(charset);
}

masterThread::~masterThread()
{
}

void masterThread::operator()()
{
	do
	{
		if(!silent)
		{
			writeIterations();
		}

		delay(interval);

	} while(!success);

	printResult();
}

void masterThread::writeIterations()
{
	std::stringstream ss;
	std::string output;

	ss << getIterations();
	ss >> output;

	std::cout << output << " iterations" << std::endl;
}

void masterThread::printResult()
{
	time_t endTime = ((unsigned)time(NULL));

	//This needs to be reworked
	time_t hours = ((endTime - startTime) / 3600);
	time_t minutes = (((endTime - startTime) / 60) - (hours * 60));
	time_t seconds = (endTime - startTime) % 3600;

	std::cout << std::endl << "Successful Attack! Cracked password is: " << crackedPassword << std::endl
		<< "Attack duration: " << getIterations() << " iterations." << std::endl
		<< "Completed in: " << hours << " hours, " << minutes << " minutes, and " << seconds
		<< " seconds. " << std::endl;
			
	if(endTime - startTime != 0)
	{
		std::cout << "Speed of this run: " << (getIterations() / (endTime - startTime)) << " Iterations per second." << std::endl;
	}
}

//Great reliable function for delaying something; resolution is in seconds
void masterThread::delay(time_t seconds)
{
	time_t startTime = time(NULL);

	do
	{
	} while(time(NULL) < startTime + seconds);
}

bool masterThread::getSuccess()
{
	return success;
}

void masterThread::incrementIterations()
{
	++iterations;
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

void masterThread::increaseIterations(int input)
{
	iterations += input;
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