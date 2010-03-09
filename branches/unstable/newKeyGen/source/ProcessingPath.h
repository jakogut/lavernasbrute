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

#include <boost/unordered_map.hpp>
#include <boost/thread/thread.hpp>

#include "MasterThread.h"
#include "MD4.h"

#include "KeyGenerator.h"

typedef boost::unordered_map<int64_pair, std::string> targetMap;

class processingPath
{
	public:

	processingPath();
	~processingPath();

	// The entry into the thread
	virtual void operator()() = 0;

	// The loop used to search the keyspace
	virtual void searchKeyspace() = 0;
    
    // Functions necessary for the Director to do its job
	virtual int getThreadID() = 0;

	virtual unsigned long long getKeyspaceEnd() = 0;
	virtual unsigned long long getKeyspaceBegin() = 0;
	virtual unsigned long long getKeyLocation() = 0;

	virtual void moveKeyspaceEnd(unsigned long long input) = 0;
	virtual void moveKeyspaceBegin(unsigned long long input) = 0;
	virtual void moveKeylocation(unsigned long long input) = 0;

    // Processing path options
	static void pushTarget(std::string input);
	static void setMaxChars(int input);
	static int getMaxChars();

	static int getNumTargets();

protected:

	static void removeTarget(targetMap::iterator it);

	static targetMap targets;
	static boost::mutex targetsMutex;

	static int maxChars;

	int remainingTargets;
};

#endif
