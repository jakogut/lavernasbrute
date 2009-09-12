#include "StreamOperations.h"

streamOperations::streamOperations(int chunkSize)
: chunkSize(chunkSize)
{
    streamSize = new unsigned int(chunkSize);

    stream = new Stream<unsigned int>(1, streamSize);
    result = new Stream<unsigned int>(1, streamSize);
}

streamOperations::~streamOperations()
{
}

void streamOperations::writeStream(unsigned int* input)
{
	streamRead(*stream, input);
}

void streamOperations::writeStream(Stream<unsigned int> input)
{
	*stream = input;
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
	writeStream(*ops.getStreamPtr());
}

void streamOperations::operator =(Stream<unsigned int> input)
{
	writeStream(input);
}

streamOperations streamOperations::operator +(streamOperations ops)
{
	Stream<unsigned int> temp(1, streamSize);
	temp = *ops.getStreamPtr();
	//ops.cleanUp();

	streamOperations returnOps(chunkSize);
	gpu_ADD(*stream, temp, *returnOps.getStreamPtr());

	return returnOps;
}

streamOperations streamOperations::operator +(int operand)
{
	streamOperations returnOps(chunkSize);
	gpu_ADD_CONST(*stream, operand, *returnOps.getStreamPtr());

	return returnOps;
}

streamOperations streamOperations::operator -(streamOperations ops)
{
	Stream<unsigned int> temp(1, streamSize);
	temp = *ops.getStreamPtr();
	//ops.cleanUp();

	streamOperations returnOps(chunkSize);
	gpu_SUB(*stream, temp, *returnOps.getStreamPtr());

	return returnOps;
}

streamOperations streamOperations::operator /(streamOperations ops)
{
	Stream<unsigned int> temp(1, streamSize);
	temp = *ops.getStreamPtr();
	//ops.cleanUp();

	streamOperations returnOps(chunkSize);
	gpu_DIV(*stream, temp, *returnOps.getStreamPtr());

	return returnOps;
}

streamOperations streamOperations::operator *(streamOperations ops)
{
	Stream<unsigned int> temp(1, streamSize);
	temp = *ops.getStreamPtr();
	//ops.cleanUp();

	streamOperations returnOps(chunkSize);
	gpu_MUL(*stream, temp, *returnOps.getStreamPtr());

	return returnOps;
}

void streamOperations::operator +=(streamOperations ops)
{
	Stream<unsigned int> temp(1, streamSize);
	temp = *ops.getStreamPtr();
	//ops.cleanUp();

	gpu_ADD(*stream, temp, *result);
	this->writeStream(*result);
}

void streamOperations::operator +=(int operand)
{
	gpu_ADD_CONST(*stream, operand, *result);
	this->writeStream(*result);
}

void streamOperations::operator -=(streamOperations ops)
{
	Stream<unsigned int> temp(1, streamSize);
	temp = *ops.getStreamPtr();
	//ops.cleanUp();

	gpu_SUB(*stream, temp, *result);
	this->writeStream(*result);
}

void streamOperations::operator /=(streamOperations ops)
{
	Stream<unsigned int> temp(1, streamSize);
	temp = *ops.getStreamPtr();

	//ops.cleanUp();
	gpu_DIV(*stream, temp, *result);
	this->writeStream(*result);
}

void streamOperations::operator *=(streamOperations ops)
{
	Stream<unsigned int> temp(1, streamSize);
	temp = *ops.getStreamPtr();
	//ops.cleanUp();

	gpu_MUL(*stream, temp, *result);
	this->writeStream(*result);
}

streamOperations streamOperations::operator ~()
{
	streamOperations returnOps(chunkSize);
	gpu_NOT(*stream, *returnOps.getStreamPtr());
	return returnOps;
}

streamOperations streamOperations::operator |(streamOperations ops)
{
	Stream<unsigned int> temp(1, streamSize);
	temp = *ops.getStreamPtr();
	//ops.cleanUp();

	streamOperations returnOps(chunkSize);
	gpu_OR(*stream, temp, *returnOps.getStreamPtr());

	return returnOps;
}

streamOperations streamOperations::operator ^(streamOperations ops)
{
	Stream<unsigned int> temp(1, streamSize);
	temp = *ops.getStreamPtr();
	//ops.cleanUp();

	streamOperations returnOps(chunkSize);
	gpu_XOR_BIOP(*stream, temp, *returnOps.getStreamPtr());

	return returnOps;
}

streamOperations streamOperations::operator &(streamOperations ops)
{
	Stream<unsigned int> temp(1, streamSize);
	temp = *ops.getStreamPtr();
	//ops.cleanUp();

	streamOperations returnOps(chunkSize);
	gpu_AND(*stream, temp, *returnOps.getStreamPtr());

	return returnOps;
}

streamOperations streamOperations::operator <<(int shift)
{
	streamOperations returnOps(chunkSize);
	gpu_LSHIFT(*stream, shift, *returnOps.getStreamPtr());

	return returnOps;
}

streamOperations streamOperations::operator >>(int shift)
{
	streamOperations returnOps(chunkSize);
	gpu_RSHIFT(*stream, shift, *returnOps.getStreamPtr());

	return returnOps;
}

void streamOperations::cleanUp()
{
	if(stream)
	{
		delete stream;
		stream = NULL;
	}

	if(result)
	{
		delete result;
		result = NULL;
	}

	if(streamSize)
	{
		delete streamSize;
		streamSize = NULL;
	}
}