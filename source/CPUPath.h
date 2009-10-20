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

	int getThreadID();

	unsigned long long getKeyspaceEnd();
	unsigned long long getKeyspaceBegin();
	unsigned long long getKeyLocation();

	void moveKeyspaceEnd(unsigned long long input);
	void moveKeyspaceBegin(unsigned long long input);
	void moveKeylocation(unsigned long long input);

protected:

	int id;

	NTLM ntlm;

	//Function for converting an integer to a text string (key)
	inline void integerToKey(unsigned long long location);

	std::string currentKey, hashedKey;

	unsigned long long keyspaceSize, keyspaceEnd, keyspaceBegin, keyLocation, lookupSize;
	long localProgress;
};

#endif