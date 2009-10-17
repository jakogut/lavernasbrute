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
	static void setSuccess(bool input);
	static void setInterval(int input);
	static bool getSilent();
	static void setSilent(bool input);

	static unsigned long long getIterations();
	static void incrementIterations();
	static void increaseIterations(long input);

	static char* getCharset();
	static int getCharsetLength();

	static void setLargeLookup(bool input);
	static void disableLookup(bool input);

	static char** getLookup();
	static unsigned long long getLookupSize();

	static void setRandomizeCharset(bool input);

	void printResult();

protected:

	int id;
	time_t startTime;

	static unsigned long long iterations;
	static int interval;
	static bool silent;

	//When a thread is successful, it sets this value to true, signaling the other threads
	//That the job is done.
	static bool success;

	//Randomize the character set in order to prevent prediction of keyspace searches
	static bool randomizeCharset;

	static bool largeLookup, lookupDisabled;

	static char* charset;
	static int charsetLength;

	static char** integerToKeyLookup;
	static unsigned long long lookupSize;

	std::string randomizedCharset;
};

#endif