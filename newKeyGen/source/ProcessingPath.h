//Part of Laverna's Brute
//Base processing path which path implementations inherit from.

#ifndef PROCESSINGPATH_H_
#define PROCESSINGPATH_H_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string>

#include <boost/thread/thread.hpp>

#include "Version.h"
#include "BloomFilter.h"
#include "KeyGenerator.h"
#include "BinarySearch.h"

#include "MD4.h"

bool operator<(const hashContext& a, const hashContext& b);

class processingPath
{
	public:

	processingPath();
	~processingPath();

	// The entry into the thread
	virtual void operator()() = 0;

	// The loop used to search the keyspace
	virtual void searchKeyspace() = 0;
    
    // Functions necessary for the master thread to do its job
	virtual unsigned int getThreadID() = 0;

	virtual unsigned long long getKeyspaceEnd() = 0;
	virtual unsigned long long getKeyspaceBegin() = 0;
	virtual unsigned long long getKeyLocation() = 0;

	virtual void moveKeyspaceEnd(unsigned long long input) = 0;
	virtual void moveKeyspaceBegin(unsigned long long input) = 0;
	virtual void moveKeylocation(unsigned long long input) = 0;

	static void createBloomFilter();
	static void setBloomFilterSize(size_t size);
	static void destroyBloomFilter();

    // Processing path options
	static void addTarget(std::string& input);
	static void setMaxChars(int input);
	static int getMaxChars();

	static int getNumTargets();

	// Set the hash type for the run
	static void setHashType(std::string type);

	static void setCharset(characterSet* charset);

	static unsigned long long calculateKeyspaceSize(int charsetLength, int keyLength);
	static unsigned long long calculateKeyspaceSize();

protected:

	int remainingTargets;

	static std::vector<hashContext> targets;
	static unsigned int targetsCracked;

	static bloomFilter* bFilter;
	static size_t bFilterSize;

	static characterSet* pCharset;
	static int maxChars;

	static std::string hashType;
};

#endif
