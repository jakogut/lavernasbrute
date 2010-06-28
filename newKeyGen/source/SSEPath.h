// Part of Laverna's Brute

#ifdef SSE

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

	unsigned int getThreadID();

	unsigned long long getKeyspaceEnd();
	unsigned long long getKeyspaceBegin();
	unsigned long long getKeyLocation();

	void moveKeyspaceEnd(unsigned long long input);
	void moveKeyspaceBegin(unsigned long long input);
	void moveKeylocation(unsigned long long input);

protected:

	int id;

	char* currentKeys[12];
	hashContext_MD4 weakHashedKeys[12];

	targetMap::iterator targetIterator;

	unsigned long long keyspaceBegin, keyspaceEnd, keyLocation;
};

#endif

#endif
