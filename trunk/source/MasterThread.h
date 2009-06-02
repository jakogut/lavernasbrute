//Part of Laverna's Brute

#ifndef MASTERTHREAD_H_
#define MASTERTHREAD_H_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class masterThread
{
public:

	masterThread(int id);
	~masterThread();

	void operator()();

	static bool getSuccess();
	static void setSuccess(bool boolean);
	static void printToConsole(string print);
	static void writeIterations();
	static int getInterval();
	static void setInterval(int input);
	static bool getSilent();
	static void setSilent(bool input);

	static long long getIterations();
	static void incrementIterations();
	static void increaseIterations(int input);

	static void setCrackedPassword(string input);

	static char* getCharset();
	static int getCharsetLength();

	void printResult();
	void delay(time_t seconds);

protected:

	static string target;

	int id;
	time_t startTime;

	static long long iterations;
	static int interval;
	static bool silent;

	//When a thread is successful, it sets this value to true, signaling to the other threads
	//That the job is done.
	static bool success;

	//static boost::try_mutex SuccessMutex;
	//static boost::mutex printMutex;

	static string crackedPassword;

	static char* charset;
	static int charsetLength;
};

#endif