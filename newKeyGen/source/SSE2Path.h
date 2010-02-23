// Part of Laverna's Brute

#ifndef SSE2PATH_H_
#define SSE2PATH_H_

#include "Director.h"
#include "ProcessingPath.h"

#include "NTLM_SSE2.h"

class SSE2Path : protected processingPath
{
public:

	SSE2Path(int id);
	~SSE2Path();

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

	std::string currentKeys[12];
	int64_pair weakHashedKeys[12];

	targetMap::iterator targetIterator;

	unsigned long long keyspaceBegin, keyspaceEnd, keyLocation;
};

#endif
