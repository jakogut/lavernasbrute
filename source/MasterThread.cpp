//Part of Laverna's Brute

#include "MasterThread.h"

masterThread::masterThread() 
{
	//Start the clock
	startTime = time(NULL);

	charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	charsetLength = (int)(strlen(charset));

	if(randomizeCharset)
	{
		srand(time(NULL));

		std::string::iterator charsetIterator;
		std::string originalCharset = charset;

		while(originalCharset.length() > 0)
		{
			int randomChar = (rand() % originalCharset.length());
			randomizedCharset.append(originalCharset.substr(randomChar, 1));

			charsetIterator = originalCharset.begin() + randomChar;
			originalCharset.erase(charsetIterator);
		}

		charset = (char*)randomizedCharset.c_str();
	}
}

masterThread::~masterThread()
{
}

void masterThread::operator()()
{
	boost::posix_time::seconds updateInterval(interval);

	while(!success)
	{
		if(!silent)
			std::cout << getIterations() << " iterations" << std::endl;

		boost::this_thread::sleep(updateInterval);
	}

	printResult();
}

void masterThread::printResult()
{
	time_t endTime = ((unsigned)time(NULL));

	//This needs to be reworked
	time_t hours = (endTime - startTime) / 3600;
	time_t minutes = (endTime - startTime) / 60;
	time_t seconds = (endTime - startTime);

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
	++iterations;
}

void masterThread::setSuccess(bool input)
{
	success = input;
}

char* masterThread::getCharset()
{
	return charset;
}

int masterThread::getCharsetLength()
{
	return charsetLength;
}

void masterThread::setRandomizeCharset(bool input)
{
	randomizeCharset = input;
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

void masterThread::setSilent(bool input)
{
	silent = input;
}

void masterThread::setInterval(int input)
{
	interval = input;
}

void masterThread::setCrackedPassword(std::string input)
{
	crackedPassword = input;
}