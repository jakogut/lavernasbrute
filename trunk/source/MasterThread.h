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

	masterThread(bool largeLookup);
	~masterThread();

	void operator()();

	static bool getSuccess();
	static void setSuccess(bool input);
	static void setInterval(int input);
	static bool getSilent();
	static void setSilent(bool input);

	static long long getIterations();
	static void incrementIterations();
	static void increaseIterations(int input);

	static void setCrackedPassword(std::string input);

	static char* getCharset();
	static int getCharsetLength();

	static char** getLookup();
	static long getLookupSize();

	static void setRandomizeCharset(bool input);

	void printResult();

protected:

	static char* target;

	int id;
	time_t startTime;

	static long long iterations;
	static int interval;
	static bool silent;

	//When a thread is successful, it sets this value to true, signaling the other threads
	//That the job is done.
	static bool success;

	//Randomize the character set in order to prevent prediction of keyspace searches
	static bool randomizeCharset;

	//When the plantext string to the hsah is cracked, it is stored here
	static std::string crackedPassword;

	static char* charset;
	static int charsetLength;

	static char** integerToKeyLookup;
	static long lookupSize;

	std::string randomizedCharset;
};

#endif
