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

	// The loop that is run to crack a given target hash
	virtual void operator()() = 0;
    
    // Functions necessary for the Director to do its job
	virtual int getThreadID();

	virtual unsigned long long getKeyspaceEnd();
	virtual unsigned long long getKeyspaceBegin();
	virtual unsigned long long getKeyLocation();

	virtual void moveKeyspaceEnd(unsigned long long input);
	virtual void moveKeyspaceBegin(unsigned long long input);
	virtual void moveKeylocation(unsigned long long input);

    // Processing path options
	static void pushTarget(std::string input);
	static void setMaxChars(int input);

	static int getNumTargets();
	static std::map<std::string, std::string> getTargets();

protected:

	static int maxChars;
	static int numWorkers;

	static std::map<std::string, std::string> targets;

	char* charset;
	int charsetLength;

	char** integerToKeyLookup;
};

#endif