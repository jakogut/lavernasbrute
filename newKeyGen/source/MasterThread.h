//Part of Laverna's Brute

#ifndef MASTERTHREAD_H_
#define MASTERTHREAD_H_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sstream>
#include <vector>

#include <boost/thread/thread.hpp>
#include <boost/date_time.hpp>

class masterThread
{
public:

	masterThread();
	~masterThread();

	void operator()();

	static unsigned long long pow(unsigned long long base, unsigned long long power);

	static bool getSuccess();
	static void setSuccess();

	static bool getSilent();
	static void setSilent(bool input);

	static int getNumWorkers();
	static void setNumWorkers(int input);

	static void setRemainingTargets(int input);

	static unsigned long long getIterations();
	static void setIterations(unsigned long long input);
	static void incrementIterations();
	static void increaseIterations(unsigned int input);

	static char* getCharset();
	static int getCharsetLength();

	static void setRandomizeCharset(bool input);
	static void setFrequencyCharset(bool input);

	static void printResult(std::string hash, std::string plaintext);

	void printStatistics();

protected:

	int id;
	time_t startTime;

	static unsigned long long iterations;
	static bool silent;

	static int numWorkers;

	//When a thread is successful, it sets this value to true, signaling the other threads
	//That the job is done.
	static bool success;

	// Randomize the character set in order to prevent prediction of keyspace searches
	static bool randomizeCharset;

	// Use a character set organized by letter frequency.
	static bool frequencyCharset;

	static char* charset;
	static int charsetLength;

	static int remainingTargets;

	std::string randomizedCharset;

	static std::vector< std::pair<std::string, std::string> > results;

	static boost::mutex stdoutMutex;
	static boost::mutex iterationsMutex;
};

#endif
