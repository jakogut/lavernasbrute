//Part of Laverna's Brute

#include "MasterThread.h"

masterThread::masterThread(int id) 
: id(id)
{
	//Start the clock
	startTime = time(NULL);

	charset = (char *)"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	charsetLength = strlen(charset);
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
			delay(interval);
			writeIterations();
		}

	} while(!getSuccess());

	printResult();
}

bool masterThread::getSuccess()
{
	return success;
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

void masterThread::incrementIterations()
{
	++iterations;
}

void masterThread::increaseIterations(int input)
{
	iterations += input;
}

void masterThread::writeIterations()
{
	stringstream ss;
	string output;

	ss << getIterations();
	ss >> output;

	cout << output << " iterations" << endl;
}

void masterThread::printResult()
{
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

//Great reliable function for delaying something; resolution is in seconds
void masterThread::delay(time_t seconds)
{
	time_t startTime = time(NULL);

	do
	{
	} while(time(NULL) < startTime + seconds);
}

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
