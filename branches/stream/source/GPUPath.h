//Part of Laverna's Brute

#ifndef GPUPATH_H_
#define GPUPATH_H_

#include "ProcessingPath.h"
#include "NTLM_GPU.h"
#include <math.h>

class GPUPath : protected processingPath
{
public:

	GPUPath(int id);
	~GPUPath();

	void operator()();

protected:

	int id;

	void integerToKey();
	std::string *keyBuffer, *hashBuffer;

	NTLM_GPU* ntlm;

	unsigned long long keyspaceSize, startKeyspace, endKeyspace, keyLocation;
	long expandedLookupSize;
	int localProgress;

	int chunkSize;
};

#endif