//Part of Laverna's Brute

#ifndef CPUPATH_H_
#define CPUPATH_H_

#include "ProcessingPath.h"

class CPUPath : protected processingPath
{
public:

	CPUPath(int id);
	~CPUPath();

	void operator()();

protected:

	int id;

	NTLM ntlm;

	//Function for converting an integer to a text string (key)
	inline void integerToKey(unsigned long long location);

	std::string currentKey, hashedKey;

	unsigned long long keyspaceSize, startKeyspace, endKeyspace, keyLocation, lookupSize;
	long localProgress;
};

#endif