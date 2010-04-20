//Part of Laverna's Brute

#include "MasterThread.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

bool masterThread::success = 0;
bool masterThread::silent = 0;
int masterThread::numWorkers = 0;
int masterThread::remainingTargets = 0;
unsigned long long masterThread::iterations = 0;
std::vector< std::pair<std::string, std::string> > masterThread::results;
boost::mutex masterThread::stdoutMutex;
boost::mutex masterThread::iterationsMutex;
characterSet masterThread::charset;


////////////////////////////////////////////

masterThread::masterThread()
{
	// Start the clock
	startTime = time(NULL);
}

masterThread::~masterThread()
{
}

void masterThread::operator()()
{
	boost::posix_time::seconds updateInterval(1);
	std::cout.precision(3);

	do
	{
		boost::this_thread::sleep(updateInterval);

		if(!silent)
		{
			boost::mutex::scoped_lock lock(stdoutMutex);

			std::cout.flush();

			std::cout << "\rAverage speed: " << ((getIterations() / (time(NULL) - startTime)) / 1000000.0f) << " M keys/s"
			   << "\tHashes Remaining: " << remainingTargets;

			std::cout.flush();
		}

	} while(!success);

	printStatistics();
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

	std::cout.precision(3);

	std::cout << "\n\nThe run has completed!"
		<< "\nAttack duration: " << getIterations() << " iterations."
		<< "\nCompleted in: " << days << " days, " << hours << " hours, "
		<< minutes << " minutes, and " << seconds << " seconds. ";

	if(silent)
	{
		std::cout << "\n\nAverage speed: " << ((getIterations() / (time(NULL) - startTime)) / 1000000.0f) << " M keys/s" << std::endl;
		std::cout.flush();
	}
	else
	{
		std::cout << std::endl;
	}
}

bool masterThread::getSuccess()
{
	return success;
}

void masterThread::setSuccess()
{
	success = true;
}

int masterThread::getNumWorkers()
{
	return numWorkers;
}

void masterThread::setNumWorkers(int input)
{
	numWorkers = input;
}

void masterThread::increaseNumWorkers(int input)
{
	numWorkers += input;
}

void masterThread::setRemainingTargets(int input)
{
	remainingTargets = input;
}

unsigned long long masterThread::getIterations()
{
	return iterations;
}

void masterThread::setIterations(unsigned long long input)
{
	iterations = input;
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

void masterThread::initCharset(unsigned int len, int min, int max, 
							   int charsec0, int charsec1, int charsec2, int charsec3)
{
	charset.init(len, min, max, charsec0, charsec1, charsec2, charsec3);
}

characterSet* masterThread::getCharset()
{
	return &charset;
}

void masterThread::printResult(std::string hash, std::string plaintext)
{

	boost::mutex::scoped_lock lock(stdoutMutex);

	if(!silent)
	{
		// Clear the last line
		std::cout << "\r";
		std::cout.flush();
	}

	std::cout << hash << " == ";

	std::cout.width(80);

	std::cout << std::left << plaintext << std::endl << std::endl;

	std::cout.flush();
}

bool masterThread::getSilent()
{
	return silent;
}

void masterThread::setSilent(bool input)
{
	silent = input;
}
