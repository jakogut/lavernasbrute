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
#include "NTLM.h"

class processingPath
{
	public:

	processingPath();
	~processingPath();

	//The loop that is run to crack a given target hash
	virtual void operator()() = 0;

	static std::string getTarget();
	static void setTarget(std::string input);
	static void setMaxChars(int input);
	static void setTotalThreads(int input);

protected:

	static int maxChars;
	static int totalThreads;
	static std::string target;

	std::string* charset;
	int charsetLength;

	std::string* integerToKeyLookup;

	//This must be defined for every processing path
	virtual void integerToKey(unsigned long long location) = 0;
};

#endif