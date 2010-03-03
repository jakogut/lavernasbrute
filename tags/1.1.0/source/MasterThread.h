//Part of Laverna's Brute

#ifndef MASTERTHREAD_H_
#define MASTERTHREAD_H_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <iomanip>

#include <boost/thread/thread.hpp>
#include <boost/date_time.hpp>

#include "KeyGenerator.h"

class masterThread
{
public:

	masterThread();
	~masterThread();

	void operator()();

	static bool getSuccess();
	static void setSuccess();

	static bool getSilent();
	static void setSilent(bool input);

	static int getNumWorkers();
	static void setNumWorkers(int input);
	static void increaseNumWorkers(int input);

	static void setRemainingTargets(int input);

	static unsigned long long getIterations();
	static void setIterations(unsigned long long input);
	static void incrementIterations();
	static void increaseIterations(unsigned int input);

	static void initCharset(unsigned int len, int min, int max, 
							int charsec0, int charsec1, int charsec2, int charsec3);

	static characterSet* getCharset();

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

	static characterSet charset;

	static int remainingTargets;

	static std::vector< std::pair<std::string, std::string> > results;

	static boost::mutex stdoutMutex;
	static boost::mutex iterationsMutex;
};

#endif
