//Part of Laverna's Brute

#ifndef CPUPATH_H_
#define CPUPATH_H_

#include "ProcessingPath.h"
#include "MasterThread.h"

using namespace std;

class CPUPath : public processingPath
{
public:

	CPUPath(int id);
	~CPUPath();

	void operator()();

protected:

	int id;

	char* charset;
	int charsetLength;

	unsigned long long keyLocation;
};

#endif
