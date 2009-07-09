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

	std::string currentKey;

	unsigned long long startKeyspace, endKeyspace, keyLocation;
};

#endif
