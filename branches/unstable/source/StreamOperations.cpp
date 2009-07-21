#include "StreamOperations.h"

streamOperations::streamOperations(long long chunkSize, int numStreams)
: chunkSize(chunkSize), numStreams(numStreams)
{
	int streamSize[] = {(unsigned int)chunkSize};

	output = new int[(unsigned int)chunkSize];

	stream = new Stream<int>*[numStreams];

	for(int i = 0; i < numStreams; i++)
	{
		stream[i] = new Stream<int>(1, (unsigned int*)streamSize);
	}
}

streamOperations::~streamOperations()
{
	for(int i = 0; i < numStreams; i++)
	{
		delete stream[i];
	}

	delete [] stream;
}

void streamOperations::writeStream(int streamIndex, int* input)
{
	streamRead(*stream[streamIndex], input);
}

int* streamOperations::readStream(int streamIndex)
{
	streamWrite(*stream[streamIndex], output);

	return output;
}

void streamOperations::ADD_CONST(int streamIndex, int input, int outputStream)
{
	gpu_ADD_CONST(*stream[streamIndex], input, *stream[outputStream]);
}

void streamOperations::ADD(int streamIndex0, int streamIndex1, int outputStream)
{
	gpu_ADD(*stream[streamIndex0], *stream[streamIndex1], *stream[outputStream]);
}

void streamOperations::SUB(int streamIndex0, int streamIndex1, int outputStream)
{
	gpu_SUB(*stream[streamIndex0], *stream[streamIndex1], *stream[outputStream]);
}

void streamOperations::DIV(int streamIndex0, int streamIndex1, int outputStream)
{
	gpu_DIV(*stream[streamIndex0], *stream[streamIndex1], *stream[outputStream]);
}

void streamOperations::MUL(int streamIndex0, int streamIndex1, int outputStream)
{
	gpu_MUL(*stream[streamIndex0], *stream[streamIndex1], *stream[outputStream]);
}

void streamOperations::NOT(int streamIndex, int outputStream)
{
	gpu_NOT(*stream[streamIndex], *stream[outputStream]);
}

void streamOperations::OR(int streamIndex0, int streamIndex1, int outputStream)
{
	gpu_OR(*stream[streamIndex0], *stream[streamIndex1], *stream[outputStream]);
}

void streamOperations::XOR(int streamIndex0, int streamIndex1, int outputStream)
{
	gpu_XOR_BIOP(*stream[streamIndex0], *stream[streamIndex1], *stream[outputStream]);
}

void streamOperations::XOR(int streamIndex0, int streamIndex1, int streamIndex2, int outputStream)
{
	gpu_XOR_TRIOP(*stream[streamIndex0], *stream[streamIndex1], *stream[streamIndex2], *stream[outputStream]);
}

void streamOperations::AND(int streamIndex0, int streamIndex1, int outputStream)
{
	gpu_AND(*stream[streamIndex0], *stream[streamIndex1], *stream[outputStream]);
}

void streamOperations::LSHIFT(int streamIndex, int shift, int outputStream)
{
	gpu_LSHIFT(*stream[streamIndex], shift, *stream[outputStream]);
}

void streamOperations::RSHIFT(int streamIndex, int shift, int outputStream)
{
	gpu_RSHIFT(*stream[streamIndex], shift, *stream[outputStream]);
}