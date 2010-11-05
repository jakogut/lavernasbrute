// Part of Laverna's Brute

#ifndef CUDA_PATH_H_
#define CUDA_PATH_H_

#include "ProcessingPath.h"
#include "MasterThread.h"

#include "hashing/MD4_CUDA.cuh"
#include "hashing/MD4.h"

class CUDAPath : protected processingPath
{
public:

	CUDAPath(int id);
	~CUDAPath();

	void operator()();
	void searchKeyspace();

	unsigned int getThreadID();

	unsigned long long getKeyspaceEnd();
	unsigned long long getKeyspaceBegin();
	unsigned long long getKeyspaceLocation();

	void moveKeyspaceEnd(unsigned long long input);
	void moveKeyspaceBegin(unsigned long long input);
	void moveKeyspaceLocation(unsigned long long input);

protected:

	int id;

	std::vector<hashContext>::iterator targetIterator;

	MD4_CUDA* cu_md4;
	MD4 md4;

	unsigned long long keyspaceEnd, keyspaceBegin, keyLocation;

	hashContext* ctx;
};

#endif

