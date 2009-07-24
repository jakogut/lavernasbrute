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
	static void setSuccess(bool boolean);
	static void printToConsole(std::string print);
	static void writeIterations();
	static void setInterval(int input);
	static bool getSilent();
	static void setSilent(bool input);

	static long long getIterations();
	static void incrementIterations();
	static void increaseIterations(unsigned long long input);

	static void setCrackedPassword(std::string input);

	static char* getCharset();
	static int getCharsetLength();

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

	static std::string crackedPassword;

	static char* charset;
	static int charsetLength;
};

#endif
