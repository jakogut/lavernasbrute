//Part of Laverna's Brute

#ifndef MASTERTHREAD_H_
#define MASTERTHREAD_H_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>

using namespace std;

class masterThread
{
public:

	masterThread(int id, time_t time);
	~masterThread();

	void operator()();

	static bool getSuccess();
	static void setSuccess(bool boolean);
	static void printToConsole(string print);
	static void writeIterations();
	static int getInterval();
	static void setInterval(int write);
	static bool getSilent();
	static void setSilent(bool write);

	static long long getIterations();
	static void incrementIterations();
	static void increaseIterations(int input);

	static void setCrackedPassword(string write);

	static char* getCharset();
	static int getCharsetLength();

	static string passwd;

	void printResult();

	int id;
	time_t startTime;

	static long long iterations;
	static int interval;
	static bool silent;

	//When a thread is successful, it sets this value to true, signaling to the other threads
	//That the job is done.
	static bool success;

	static boost::try_mutex SuccessMutex;
	static boost::mutex printMutex;

	static string crackedPassword;

	static char* randCharset;
	static int charsetLength;
};

#endif