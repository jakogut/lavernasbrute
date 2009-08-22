//Part of Laverna's Brute

#include "MasterThread.h"

masterThread::masterThread(bool largeLookup) 
{
	//Set the charset and charset length variables
	charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	charsetLength = (int)(strlen(charset));

	//If specified, randomize the order of the character set
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

	//Create the integer to key conversion lookup array
	int lookupChars;
	largeLookup ? lookupChars = 4 : lookupChars = 3;

	lookupSize = pow((double)charsetLength, lookupChars);
	integerToKeyLookup = new char*[lookupSize];

	for(int i = 0; i < lookupSize; i++)
	{
		unsigned long long num = i;
		int j = 0;

		integerToKeyLookup[i] = new char[lookupChars + 1];

		for(; num > 0; j++)
		{
			integerToKeyLookup[i][j] = charset[num % (charsetLength)];
			num /= charsetLength;
		}

		integerToKeyLookup[i][lookupChars] = '\0';
	}

	//Start the clock
	startTime = time(NULL);
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

	//Clean up
	delete [] *integerToKeyLookup;
	delete [] integerToKeyLookup;
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

char** masterThread::getLookup()
{
	return integerToKeyLookup;
}

long masterThread::getLookupSize()
{
	return lookupSize;
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
	crackedPassword.assign(input);
}