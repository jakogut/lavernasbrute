#include "StreamOperations.h"

streamOperations::streamOperations(int chunkSize)
: chunkSize(chunkSize)
{
	int streamSize[] = {chunkSize};

	inputStream0 = new Stream<int>(1, (unsigned int*)streamSize);
	inputStream1 = new Stream<int>(1, (unsigned int*)streamSize);
	inputStream2 = new Stream<int>(1, (unsigned int*)streamSize);

	outputStream = new Stream<int>(1, (unsigned int*)streamSize);

	output = new int[chunkSize];
}

streamOperations::~streamOperations()
{
	delete inputStream0;
	delete inputStream1;
	delete inputStream2;
	delete outputStream;
	delete output;
}

void streamOperations::writeStream(int stream, int input[])
{
	switch(stream)
	{
	case 0:
		streamRead(*inputStream0, input);
		break;

	case 1:
		streamRead(*inputStream1, input);
		break;

	case 2:
		streamRead(*inputStream2, input);
		break;
	}
}

int* streamOperations::readResult()
{
	return output;
}

void streamOperations::ADD(int input1)
{
	gpu_ADD_CONST(*inputStream0, input1, *outputStream);
}

void streamOperations::ADD()
{
	gpu_ADD(*inputStream0, *inputStream1, *outputStream);
}

void streamOperations::SUB()
{
	gpu_SUB(*inputStream0, *inputStream1, *outputStream);
}

void streamOperations::DIV()
{
	gpu_DIV(*inputStream0, *inputStream1, *outputStream);
}

void streamOperations::MUL()
{
	gpu_MUL(*inputStream0, *inputStream1, *outputStream);
}

void streamOperations::NOT()
{
	gpu_NOT(*inputStream0, *outputStream);
}

void streamOperations::OR()
{
	gpu_OR(*inputStream0, *inputStream1, *outputStream);
}

void streamOperations::XOR_BI()
{
	gpu_XOR_BIOP(*inputStream0, *inputStream1, *outputStream);
}

void streamOperations::XOR_TRI()
{
	gpu_XOR_TRIOP(*inputStream0, *inputStream1, *inputStream2, *outputStream);
}

void streamOperations::AND()
{
	gpu_AND(*inputStream0, *inputStream1, *outputStream);
}

void streamOperations::LSHIFT(int shift)
{
	gpu_LSHIFT(*inputStream0, shift, *outputStream);
}

void streamOperations::RSHIFT(int shift)
{
	gpu_RSHIFT(*inputStream0, shift, *outputStream);
}