// Part of Laverna's Brute

#ifndef SSEPath_H_
#define SSEPath_H_

#include "Director.h"
#include "ProcessingPath.h"

#include "MD4_SSE.h"

class SSEPath : protected processingPath
{
public:

	SSEPath(int id);
	~SSEPath();

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

	char* currentKeys[12];
	int64_pair weakHashedKeys[12];

	targetMap::iterator targetIterator;

	unsigned long long keyspaceBegin, keyspaceEnd, keyLocation;
};

#endif
