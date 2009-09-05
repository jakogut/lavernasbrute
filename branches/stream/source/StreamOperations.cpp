#include "StreamOperations.h"

streamOperations::streamOperations(long long chunkSize)
: chunkSize(chunkSize)
{
   streamSize = new int(chunkSize);

   stream = new Stream<unsigned int>(1, (unsigned int*)streamSize);
   result = new Stream<unsigned int>(1, (unsigned int*)streamSize);
}

streamOperations::~streamOperations()
{
}

void streamOperations::writeStream(unsigned int* input)
{
   streamRead(*stream, input);
}

void streamOperations::writeStream(Stream<unsigned int>* input)
{
   *stream = *input;
}

void streamOperations::readStream(unsigned int* output)
{
   streamWrite(*stream, output);
}

Stream<unsigned int>* streamOperations::getStreamPtr()
{
   return stream;
}

void streamOperations::operator =(streamOperations ops)
{
   writeStream(ops.getStreamPtr());
}

void streamOperations::operator =(Stream<unsigned int> input)
{
   writeStream(&input);
}

streamOperations streamOperations::operator +(streamOperations ops)
{
	streamOperations returnOps(chunkSize);
	gpu_ADD(*this->getStreamPtr(), *ops.getStreamPtr(), *returnOps.getStreamPtr());
	return returnOps;
}

streamOperations streamOperations::operator +(int operand)
{
	streamOperations returnOps(chunkSize);
	gpu_ADD_CONST(*this->getStreamPtr(), operand, *returnOps.getStreamPtr());
	return returnOps;
}

streamOperations streamOperations::operator -(streamOperations ops)
{
	streamOperations returnOps(chunkSize);
	gpu_SUB(*this->getStreamPtr(), *ops.getStreamPtr(), *returnOps.getStreamPtr());
	return returnOps;
}

streamOperations streamOperations::operator /(streamOperations ops)
{
	streamOperations returnOps(chunkSize);
	gpu_DIV(*this->getStreamPtr(), *ops.getStreamPtr(), *returnOps.getStreamPtr());
	return returnOps;
}

streamOperations streamOperations::operator *(streamOperations ops)
{
	streamOperations returnOps(chunkSize);
	gpu_MUL(*this->getStreamPtr(), *ops.getStreamPtr(), *returnOps.getStreamPtr());
	return returnOps;
}

void streamOperations::operator +=(streamOperations ops)
{
	gpu_ADD(*this->getStreamPtr(), *ops.getStreamPtr(), *result);
	this->writeStream(result);
}

void streamOperations::operator +=(int operand)
{
	gpu_ADD_CONST(*this->getStreamPtr(), operand, *result);
	this->writeStream(result);
}

void streamOperations::operator -=(streamOperations ops)
{
	gpu_SUB(*this->getStreamPtr(), *ops.getStreamPtr(), *result);
	this->writeStream(result);
}

void streamOperations::operator /=(streamOperations ops)
{
	gpu_DIV(*this->getStreamPtr(), *ops.getStreamPtr(), *result);
	this->writeStream(result);
}

void streamOperations::operator *=(streamOperations ops)
{
	gpu_MUL(*this->getStreamPtr(), *ops.getStreamPtr(), *result);
	this->writeStream(result);
}

streamOperations streamOperations::operator ~()
{
	streamOperations returnOps(chunkSize);
	gpu_NOT(*this->getStreamPtr(), *returnOps.getStreamPtr());
	return returnOps;
}

streamOperations streamOperations::operator |(streamOperations ops)
{
	streamOperations returnOps(chunkSize);
	gpu_OR(*this->getStreamPtr(), *ops.getStreamPtr(), *returnOps.getStreamPtr());
	return returnOps;
}

streamOperations streamOperations::operator ^(streamOperations ops)
{
	streamOperations returnOps(chunkSize);
	gpu_XOR_BIOP(*this->getStreamPtr(), *ops.getStreamPtr(), *returnOps.getStreamPtr());
	return returnOps;
}

streamOperations streamOperations::operator &(streamOperations ops)
{
	streamOperations returnOps(chunkSize);
	gpu_AND(*this->getStreamPtr(), *ops.getStreamPtr(), *returnOps.getStreamPtr());
	return returnOps;
}

streamOperations streamOperations::operator <<(int shift)
{
	streamOperations returnOps(chunkSize);
	gpu_LSHIFT(*this->getStreamPtr(), shift, *returnOps.getStreamPtr());
	return returnOps;
}

streamOperations streamOperations::operator >>(int shift)
{
	streamOperations returnOps(chunkSize);
	gpu_RSHIFT(*this->getStreamPtr(), shift, *returnOps.getStreamPtr());
	return returnOps;
}