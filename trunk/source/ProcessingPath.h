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
	virtual ~processingPath() = 0;

	virtual void operator()() = 0;
	
	static string getTarget();
	string generateRandString(int length);

	static void setTarget(string write);
	static void useStandardRand(bool write);

protected:

	int id;

	static int max;
	static bool randFast;
	static string target;

	MTRand* mRand;
	NTLM* mNTLM;

	char* randCharset;
	int charsetLength;
};

#endif