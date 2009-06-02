//Part of Laverna's Brute

#ifndef GPUPATH_H_
#define GPUPATH_H_

#include "ProcessingPath.h"
#include "MasterThread.h"

using namespace std;

class GPUPath : public processingPath
{
public:

	GPUPath(int id);
	~GPUPath();

	void operator()();

protected:

	int id;
	int chunkSize;

	char* charset;
	int charsetLength;
};

#endif