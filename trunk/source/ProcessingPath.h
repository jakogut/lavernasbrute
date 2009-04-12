//Part of Laverna's Brute
//Base processing path which path implementations inherit from.

#ifndef PROCESSINGPATH_H_
#define PROCESSINGPATH_H_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string>

#include "MasterThread.h"
#include "MersenneTwister.h"
#include "NTLM.h"

class processingPath
{
	public:

	processingPath();
	~processingPath();

	virtual void operator()();
	
	static int max;
	static bool randFast;
	
	//Our most oft called functions, we're inlining them for a small, but not negligible, speed improvement.
	static string getPasswd();
	string generateRandString(int length);

	static void setHash(string write);
	static void useStandardRand(bool write);

	static string passwd;

protected:

	int id;

	MTRand* mRand;
	NTLM* mNTLM;

	char* randCharset;
	int charsetLength;
};

#endif