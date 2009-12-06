//Part of Laverna's Brute

#ifndef CPUPATH_H_
#define CPUPATH_H_

#include "ProcessingPath.h"
#include "Director.h"

class CPUPath : protected processingPath
{
public:

	CPUPath(int id);
	~CPUPath();

	void operator()();
	void searchKeyspace();

	inline int getThreadID();

	unsigned long long getKeyspaceEnd();
	unsigned long long getKeyspaceBegin();
	unsigned long long getKeyLocation();

	void moveKeyspaceEnd(unsigned long long input);
	void moveKeyspaceBegin(unsigned long long input);
	void moveKeylocation(unsigned long long input);

protected:

	int id;

	NTLM ntlm;

	// Function for converting the keyspace location to a key
	inline void integerToKey(unsigned long long location);

	std::string currentKey;
	boost::unordered_map<int128, std::string>::iterator targetIterator;

	unsigned long long keyspaceSize, keyspaceEnd, keyspaceBegin, keyLocation, lookupSize;
	long localProgress;
};

#endif

