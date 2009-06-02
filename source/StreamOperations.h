//Part of Laverna's Brute

#ifndef STREAMOPERATIONS_H_
#define STREAMOPERATIONS_H_

#include <brook/brook.h>
#include <brook/stream.h>

#include "STREAM_OPS.h"

using namespace brook;

class streamOperations
{
public:

	streamOperations(int chunkSize);
	~streamOperations();

	void ADD(float input0[], float input1[], float output[]);
	void SUB(float input0[], float input1[], float output[]);
	void DIV(float input0[], float input1[], float output[]);
	void MUL(float input0[], float input1[], float output[]);

	void NOT(float input[], float output);
	void OR(float input0[], float input1[], float output[]);
	void XOR(float input0[], float input1[], float output[]);
	void AND(float input0[], float input1[], float output[]);

	void LSHIFT(float input[], int shift, float output[]);
	void RSHIFT(float input[], int shift, float output[]);

	void setChunkSize(int input);

protected:

	int chunkSize;
};

#endif
