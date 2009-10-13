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
#include <vector>

#include "MasterThread.h"
#include "NTLM.h"

class processingPath
{
	public:

	processingPath();
	~processingPath();

	static void pushTarget(std::string input);
	static void setMaxChars(int input);
	static void setTotalThreads(int input);

	static int getNumTargets();
	static std::vector<std::string> getTargets();

protected:

	unsigned long pow(unsigned long base, unsigned long power);

	static int maxChars;
	static int totalThreads;

	static std::vector<std::string> targets;

	char* charset;
	int charsetLength;

	char** integerToKeyLookup;
};

#endif