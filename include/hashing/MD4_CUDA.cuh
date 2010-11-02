// Part of Laverna's Brute

#ifndef MD4_CUDA_H_
#define MD4_CUDA_H_

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cuda.h>

#include "MD4_common.h"

#define threadsPerBlock 128
#define blocksPerGrid 1024

class MD4_CUDA
{
public:

	MD4_CUDA();
	~MD4_CUDA();

	void getHashContext(hashContext* ctx, unsigned int n);

protected:

	void initialize(hashContext* ctx, int n);

	unsigned int* wd_h;
	unsigned int* message_h;

	unsigned int* wd_d;
	unsigned int* message_d;
};

#endif

