#ifndef THREADS_H_
#define THREADS_H_

#define MIN 0		
#define MAX 100

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>

#include "MersenneTwister.h"

using namespace std;

class threads
{
public:

	threads(int id);
	~threads();

	void operator()();
	
	int id;
	static int max;
	
	//Our most oft called functions, we're inlining them for a small, but not negligible, speed improvement.
	inline static void incrementIterations();
	inline static string getPasswdNumericalString();
	inline static void setPasswdNumericalString(string write);
	inline static __int64 getIterations();
	inline static string getPasswd();
	inline string generateRandString(int length);

	static void writePasswd(string write);

	static __int64 iterations;
	static string passwdNumericalString;

	static boost::mutex IterationsMutex;
	static boost::recursive_mutex PasswdMutex;
	static boost::recursive_mutex PasswdNumericalStringMutex;
	static boost::recursive_mutex generatePasswdStringMutex;

	static string passwd;

protected:
	MTRand* mRand;
};

#endif
