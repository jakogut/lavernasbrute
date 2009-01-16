#ifndef MASTERTHREAD_H_
#define MASTERTHREAD_H_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/random.hpp>

using namespace std;

class masterThread
{
public:

	masterThread::masterThread(int id, time_t time) 
	: id(id), startTime(time)
	{
	}

	void operator()();
	static bool getSuccess();
	static void setSuccess(bool boolean);
	static void printToConsole(string print);
	static void writeIterations();

	int id;
	string passwd;
	time_t startTime;

	//When a thread is successful, it sets this value to true, signaling to the other threads
	//That the job is done.
	static bool success;

	static boost::recursive_mutex readSuccessMutex;
	static boost::recursive_mutex writeSuccessMutex;
};

#endif