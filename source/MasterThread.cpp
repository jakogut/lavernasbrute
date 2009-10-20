//Part of Laverna's Brute

#include "MasterThread.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

bool masterThread::success = 0;
bool masterThread::silent = 0;
int masterThread::numWorkers = 0;
int masterThread::interval = 0;
char* masterThread::charset = 0;
int masterThread::charsetLength = 0;
bool masterThread::randomizeCharset = 0;
char** masterThread::integerToKeyLookup = 0;
unsigned long long masterThread::lookupSize = 0;
unsigned long long masterThread::iterations = 0;
bool masterThread::largeLookup = false;
bool masterThread::lookupDisabled = false;

////////////////////////////////////////////

masterThread::masterThread() 
{
	//Set the charset and charset length variables
	charset = " 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	charsetLength = strlen(charset);

	//Check that the update interval is set within bounds
	if(interval <= 0)
		interval = 5;

	//If specified, randomize the order of the character set
	if(randomizeCharset)
	{
		srand((unsigned int)time(NULL));

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

	if(lookupDisabled)
		lookupChars = 1;
	else
		largeLookup ? lookupChars = 4 : lookupChars = 2;

	lookupSize = pow(charsetLength, lookupChars);
	integerToKeyLookup = new char*[lookupSize];

	for(long i = 0; i < lookupSize; i++)
	{
		unsigned long num = i;
		int j = 0;

		integerToKeyLookup[i] = new char[lookupChars];

		while(num > 0)
		{
			integerToKeyLookup[i][j] = charset[num % charsetLength];
			num /= charsetLength;

			j++;
		}

		// Null terminate the string
		integerToKeyLookup[i][j] = '\0';
	}

	//Start the clock
	startTime = time(NULL);
}

masterThread::~masterThread()
{
}

void masterThread::operator()()
{
	// We have two timers so that the master thread can be updated independent of the print interval.
	boost::posix_time::seconds updateInterval(1);
	time_t printInterval = interval;

	time_t startTime = time(NULL);

	while(!success)
	{
		if(!silent && (int)((time(NULL) - startTime) >= printInterval))
			std::cout << getIterations() << " iterations" << std::endl;

		startTime = time(NULL);
		boost::this_thread::sleep(updateInterval);
	}

	printResult();

	delete [] *integerToKeyLookup;
	delete [] integerToKeyLookup;
}

unsigned long long masterThread::pow(unsigned long long base, unsigned long long power)
{
	unsigned long long result = 1;

	for(unsigned long long i = 0; i < power; i++)
		result *= base;

	return result;
}

void masterThread::printResult()
{
	time_t endTime = time(NULL);
	time_t totalTime = (endTime - startTime);

	time_t seconds = totalTime % 60;
	totalTime /= 60;

	time_t minutes = totalTime % 60;
	totalTime /= 60;

	time_t hours = totalTime % 60;

	std::cout << "\nThe run has completed!"
		<< "\nAttack duration: " << getIterations() << " iterations."
		<< "\nCompleted in: " << hours << " hours, " << minutes << " minutes, and " << seconds << " seconds. " << std::endl;
			
	if(endTime - startTime != 0)
	{
		std::cout << "Speed of this run: " << (getIterations() / (endTime - startTime)) << " keys per second." << std::endl;
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

int masterThread::getNumWorkers()
{
	return numWorkers;
}

void masterThread::setNumWorkers(int input)
{
	numWorkers = input;
}

char* masterThread::getCharset()
{
	return charset;
}

int masterThread::getCharsetLength()
{
	return charsetLength;
}

void masterThread::setLargeLookup(bool input)
{
	largeLookup = input;
}

void masterThread::disableLookup(bool input)
{
	lookupDisabled = input;
}

char** masterThread::getLookup()
{
	return integerToKeyLookup;
}

unsigned long long masterThread::getLookupSize()
{
	return lookupSize;
}

void masterThread::setRandomizeCharset(bool input)
{
	randomizeCharset = input;
}

unsigned long long masterThread::getIterations()
{
	return iterations;
}

void masterThread::increaseIterations(long input)
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