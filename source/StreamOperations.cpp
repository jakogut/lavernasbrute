#include "StreamOperations.h"

streamOperations::streamOperations(int chunkSize)
: chunkSize(chunkSize)
{
}

streamOperations::~streamOperations()
{
}

//TODO: This crap needs to be cleaned up
void streamOperations::ADD(float input0[], float input1[], float output[])
{
	unsigned int streamSize[] = {chunkSize};

	Stream<float> inputStream0(1, streamSize);
	Stream<float> inputStream1(1, streamSize);
	Stream<float> outputStream(1, streamSize);

	streamRead(inputStream0, input0);
	streamRead(inputStream1, input1);

	gpu_ADD(inputStream0, inputStream1, outputStream);

	streamWrite(outputStream, output);
}

void streamOperations::SUB(float input0[], float input1[], float output[])
{
	int streamSize[] = {chunkSize};

	Stream<float> inputStream0(1, (unsigned int*)streamSize);
	Stream<float> inputStream1(1, (unsigned int*)streamSize);
	Stream<float> outputStream(1, (unsigned int*)streamSize);

	streamRead(inputStream0, input0);
	streamRead(inputStream1, input1);

	gpu_SUB(inputStream0, inputStream1, outputStream);

	streamWrite(outputStream, output);
}

void streamOperations::DIV(float input0[], float input1[], float output[])
{
	int streamSize[] = {chunkSize};

	Stream<float> inputStream0(1, (unsigned int*)streamSize);
	Stream<float> inputStream1(1, (unsigned int*)streamSize);
	Stream<float> outputStream(1, (unsigned int*)streamSize);

	streamRead(inputStream0, input0);
	streamRead(inputStream1, input1);

	gpu_DIV(inputStream0, inputStream1, outputStream);

	streamWrite(outputStream, output);
}

void streamOperations::MUL(float input0[], float input1[], float output[])
{
	int streamSize[] = {chunkSize};

	Stream<float> inputStream0(1, (unsigned int*)streamSize);
	Stream<float> inputStream1(1, (unsigned int*)streamSize);
	Stream<float> outputStream(1, (unsigned int*)streamSize);

	streamRead(inputStream0, input0);
	streamRead(inputStream1, input1);

	gpu_MUL(inputStream0, inputStream1, outputStream);

	streamWrite(outputStream, output);
}

//TODO: Finish these operators
void streamOperations::NOT(float input[], float output)
{
}

void streamOperations::OR(float input0[], float input1[], float output[])
{
}

void streamOperations::XOR(float input0[], float input1[], float output[])
{
}

void streamOperations::AND(float input0[], float input1[], float output[])
{
}

void streamOperations::LSHIFT(float input[], int shift, float output[])
{
}

void streamOperations::RSHIFT(float input[], int shift, float output[])
{
}

void streamOperations::setChunkSize(int input)
{
	chunkSize = input;
}