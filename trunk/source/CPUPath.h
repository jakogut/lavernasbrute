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
	void integerToKey(unsigned long location);

	char *currentKey, *hashedKey;

	unsigned long keyspaceSize, startKeyspace, endKeyspace, keyLocation;
	long localProgress;
};

#endif