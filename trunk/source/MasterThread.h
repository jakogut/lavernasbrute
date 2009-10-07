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

	static bool getSuccess();
	static void setSuccess(bool input);
	static void setInterval(int input);
	static bool getSilent();
	static void setSilent(bool input);

	static unsigned long getIterations();
	static void incrementIterations();
	static void increaseIterations(long input);

	static void setCrackedPassword(std::string input);

	static char* getCharset();
	static int getCharsetLength();

	static void setNumTargets(int input);
	static void pushTargetHash(std::string input);
	static void pushCrackedHash(std::string input);

	static void setLargeLookup(bool input);
	static void disableLookup(bool input);

	static std::string* getLookup();
	static long getLookupSize();

	static void setRandomizeCharset(bool input);

	void printResult();

protected:

	int id;
	time_t startTime;

	static unsigned long iterations;
	static int interval;
	static bool silent;

	//When a thread is successful, it sets this value to true, signaling the other threads
	//That the job is done.
	static bool success;

	//Randomize the character set in order to prevent prediction of keyspace searches
	static bool randomizeCharset;

	static bool largeLookup, lookupDisabled;

	static int numTargets;
	static std::vector<std::string> targetHashes;
	static std::vector<std::string> crackedHashes;

	static char* charset;
	static int charsetLength;

	static std::string* integerToKeyLookup;
	static long lookupSize;

	std::string randomizedCharset;
};

#endif