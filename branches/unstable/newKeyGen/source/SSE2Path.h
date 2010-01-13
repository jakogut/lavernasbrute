// Part of Laverna's Brute

#ifndef SSE2PATH_H_
#define SSE2PATH_H_

#include "Director.h"
#include "ProcessingPath.h"

#include "NTLM_MD.h"

class SSE2Path : protected processingPath
{
public:

	SSE2Path(int id);
	~SSE2Path();

	void operator()();
	void searchKeyspace();

	inline int getThreadID();

	unsigned long long getKeyspaceEnd();
	unsigned long long getKeyspaceBegin();
	unsigned long long getKeyLocation();

	void moveKeyspaceEnd(unsigned long long input);
	void moveKeyspaceBegin(unsigned long long input);
	void moveKeylocation(unsigned long long input);

protected:

	int id;

	std::string currentKeys[4];
	int64_pair weakHashedKeys[4];

	boost::unordered_map<int64_pair, std::string>::iterator targetIterator;

	unsigned long long keyspaceSize, keyspaceBegin, keyspaceEnd, keyLocation;
	unsigned int localProgress;
};

#endif