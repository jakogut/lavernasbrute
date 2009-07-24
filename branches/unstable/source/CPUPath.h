//Part of Laverna's Brute

#ifndef CPUPATH_H_
#define CPUPATH_H_

#include "ProcessingPath.h"
#include "Math.h"

class CPUPath : public processingPath
{
public:

	CPUPath(int id);
	~CPUPath();

	void operator()();

protected:

	int id;

	void integerToKey(unsigned long long location, char* output);

	unsigned long long startKeyspace, endKeyspace, keyLocation;
	unsigned long long localProgress;
};

#endif
