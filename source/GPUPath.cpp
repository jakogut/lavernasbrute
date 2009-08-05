//Part of Laverna's Brute

#include "GPUPath.h"

GPUPath::GPUPath(int id)
: id(id)
{
	mNTLM = new NTLM();

	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	//Size of data chunks to be processed by the GPU
	chunkSize = 8092;
}

GPUPath::~GPUPath()
{
}

void GPUPath::operator()()
{
}