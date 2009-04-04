//Part of Laverna's Brute

#ifndef THREADS_H_
#define THREADS_H_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>

#include "MersenneTwister.h"
#include "NTLM.h"

using namespace std;

class threads
{
public:

	threads(int id);
	~threads();

	void operator()();
	
	int id;
	static int max;
	static bool randFast;
	
	//Our most oft called functions, we're inlining them for a small, but not negligible, speed improvement.
	inline static void incrementIterations();
	inline static string getPasswd();
	inline string generateRandString(int length);

	static __int64 getIterations();
	static void setHash(string write);
	static void useStandardRand(bool write);

	static __int64 iterations;
	static string passwd;

protected:

	static char* RAND_CHARSET;
	static int CHARSET_LENGTH;

	MTRand* mRand;
	NTLM* mNTLM;
};

#endif
