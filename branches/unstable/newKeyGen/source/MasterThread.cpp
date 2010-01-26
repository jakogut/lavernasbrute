//Part of Laverna's Brute

#include "MasterThread.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

bool masterThread::success = 0;
bool masterThread::silent = 0;
int masterThread::numWorkers = 0;
int masterThread::remainingTargets = 0;
int masterThread::interval = 0;
char* masterThread::charset = 0;
int masterThread::charsetLength = 0;
bool masterThread::randomizeCharset = 0;
bool masterThread::frequencyCharset = 0;
unsigned long long masterThread::iterations = 0;
std::vector< std::pair<std::string, std::string> > masterThread::results;
boost::mutex masterThread::stdoutMutex;
boost::mutex masterThread::iterationsMutex;

////////////////////////////////////////////

masterThread::masterThread() 
{
	// If specified, use the letter frequency character set
	frequencyCharset ? 	charset = (char*)"eariotnscudpmhgbfywvkxzjqEARIOTNSlLCUDPMHGBFYWKVXZJQ0123456789" :
		charset = (char*)"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	// Set the length of the character set
	charsetLength = (int)strlen(charset);

	// Check that the update interval is set within bounds
	if(interval <= 0)
		interval = 5;

	// If specified, randomize the order of the character set
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

	// Start the clock
	startTime = time(NULL);
}

masterThread::~masterThread()
{
}

void masterThread::operator()()
{
	// We have two timers so that the master thread can be updated independent of the print interval.
	boost::posix_time::seconds updateInterval(1);

	std::cout.precision(3);

	time_t startTime = time(NULL);

	boost::this_thread::sleep(updateInterval);

	while(!success)
	{
		if(!silent)
		{
			boost::mutex::scoped_lock lock(stdoutMutex);

			std::cout << "Average speed: " << ((getIterations() / (time(NULL) - startTime)) / 1000000.0f) << " M keys/s"
				<< "\tHashes Remaining: " << remainingTargets << "\r";
		}

		boost::this_thread::sleep(updateInterval);
	}

	printStatistics();
}

unsigned long long masterThread::pow(unsigned long long base, unsigned long long power)
{
	unsigned long long result = 1;

	for(unsigned long long i = 0; i < power; i++)
		result *= base;

	return result;
}

void masterThread::printStatistics()
{
	time_t endTime = time(NULL);
	time_t totalTime = (endTime - startTime);

	time_t seconds = totalTime % 60;
	totalTime /= 60;

	time_t minutes = totalTime % 60;
	totalTime /= 60;

	time_t hours = totalTime % 24;
	totalTime /= 24;

	time_t days = totalTime;

	std::cout << "\nThe run has completed!"
		<< "\nAverage speed: " << ((getIterations() / (time(NULL) - startTime)) / 1000000.0f) << " M keys/s"
		<< "\nAttack duration: " << getIterations() << " iterations."
		<< "\nCompleted in: " << days << " days, " << hours << " hours, " 
		<< minutes << " minutes, and " << seconds << " seconds. " << std::endl;
}

bool masterThread::getSuccess()
{
	return success;
}

void masterThread::setSuccess()
{
	success = true;
}

void masterThread::incrementIterations()
{
	iterations++;
}

void masterThread::increaseIterations(unsigned int input)
{
	boost::mutex::scoped_lock lock(iterationsMutex);
	iterations += input;
}

int masterThread::getNumWorkers()
{
	return numWorkers;
}

void masterThread::setNumWorkers(int input)
{
	numWorkers = input;
}

void masterThread::setRemainingTargets(int input)
{
	remainingTargets = input;
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

void masterThread::setFrequencyCharset(bool input)
{
	frequencyCharset = input;
}

void masterThread::printResult(std::string hash, std::string plaintext)
{
	boost::mutex::scoped_lock lock(stdoutMutex);
	std::cout << "\r" << hash << " == " << plaintext << "\t\t\t\n\n";
}

unsigned long long masterThread::getIterations()
{
	return iterations;
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
