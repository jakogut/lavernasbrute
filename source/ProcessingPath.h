//Part of Laverna's Brute
//Base processing path which path implementations inherit from.

//This class declaration is dedicated to Tori :)

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

	virtual void operator()() = 0;
	
	static string getTarget();

	string generateRandString(int length);
	string integerToKey(long long location);

	static void setTarget(string write);
	static void setMaxChars(int write);
	static void setTotalThreads(int write);
	static void useStandardRand(bool write);
	static void useLinearSearch(bool write);

protected:

	MTRand* mRand;
	NTLM* mNTLM;

	int id;

	unsigned long long startKeyspace, endKeyspace;
	unsigned long long keyspaceOffset;

	
	static int maxChars;
	static int totalThreads;
	static bool randFast;
	static bool linearSearch;
	static string target;

	char* charset;
	int charsetLength;
};

#endif