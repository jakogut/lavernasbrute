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

	static long long getIterations();
	static void incrementIterations();
	static void increaseIterations(int input);

	static void setCrackedPassword(std::string input);

	static std::string getCharset();
	static std::string* getCharsetPtr();
	static int getCharsetLength();

	static void setLargeLookup(bool input);
	static void setDisableLookup(bool input);

	static std::string* getLookup();
	static long getLookupSize();

	static void setRandomizeCharset(bool input);

	void printResult();

protected:

	static std::string target;

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

	static bool largeLookup, disableLookup;

	//When the plantext string to the hsah is cracked, it is stored here
	static std::string crackedPassword;

	static std::string charset;
	static int charsetLength;

	static std::string* integerToKeyLookup;
	static long lookupSize;

	std::string randomizedCharset;
};

#endif
