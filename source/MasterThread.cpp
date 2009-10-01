//Part of Laverna's Brute

#include "MasterThread.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

bool masterThread::success = 0;
bool masterThread::silent = 0;
int masterThread::interval = 0;
std::string masterThread::crackedPassword = "";
std::string masterThread::charset = "";
int masterThread::charsetLength = 0;
bool masterThread::randomizeCharset = 0;
std::string* masterThread::integerToKeyLookup = 0;
long masterThread::lookupSize = 0;
long long masterThread::iterations = 0;
bool masterThread::largeLookup = false;
bool masterThread::disableLookup = false;

////////////////////////////////////////////

masterThread::masterThread() 
{
	//Set the charset and charset length variables
	charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	charsetLength = charset.length();

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

		charset = randomizedCharset;
	}

	//Create the integer to key conversion lookup array
	int lookupChars;

	if(disableLookup)
		lookupChars = 1;
	else
		largeLookup ? lookupChars = 4 : lookupChars = 2;

	lookupSize = pow((double)charsetLength, lookupChars);
	integerToKeyLookup = new std::string[lookupSize];

	for(int i = 0; i < lookupSize; i++)
	{
		unsigned long long num = i;
		int j = 0;

		while(num > 0)
		{
			integerToKeyLookup[i] += charset[num % charsetLength];
			num /= charsetLength;
		}
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

std::string masterThread::getCharset()
{
	return charset;
}

std::string* masterThread::getCharsetPtr()
{
	return &charset;
}

int masterThread::getCharsetLength()
{
	return charsetLength;
}

void masterThread::setLargeLookup(bool input)
{
	largeLookup = input;
}

void masterThread::setDisableLookup(bool input)
{
	disableLookup = input;
}

std::string* masterThread::getLookup()
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