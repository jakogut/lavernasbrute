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

	virtual void operator()() = 0;

	void integerToKey(unsigned long long* location, std::string* output);

	static std::string getTarget();
	static void setTarget(std::string write);
	static void setMaxChars(int write);
	static void setTotalThreads(int write);

protected:

	NTLM ntlm;

	int id;

	static int maxChars;
	static int totalThreads;
	static std::string target;

	char* charset;
	int charsetLength;
};

#endif