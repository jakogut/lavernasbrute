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

	char* currentKey;

	MD4 md4;
	MD4::scalarHashPtr hashFunc;

	targetMap::iterator targetIterator;

	unsigned long long keyspaceEnd, keyspaceBegin, keyLocation;
};

#endif
