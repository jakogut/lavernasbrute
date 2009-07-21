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

	streamOperations(long long chunkSize, int numStreams);
	~streamOperations();

	void setChunkSize(int input);

	void writeStream(int streamIndex, int input[]);
	int* readStream(int streamIndex);

	void ADD_CONST(int streamIndex, int input1, int outputStream);
	void ADD(int streamIndex0, int streamIndex1, int outputStream);
	void SUB(int streamIndex0, int streamIndex1, int outputStream);
	void DIV(int streamIndex0, int streamIndex1, int outputStream);
	void MUL(int streamIndex0, int streamIndex1, int outputStream);

	void NOT(int streamIndex, int outputStream);
	void OR(int streamIndex0, int streamIndex1, int outputStream);
	void XOR(int streamIndex0, int streamIndex1, int outputStream);
	void XOR(int streamIndex0, int streamIndex1, int streamIndex2, int outputStream);
	void AND(int streamIndex0, int streamIndex1, int outputStream);

	void LSHIFT(int streamIndex, int shift, int outputStream);
	void RSHIFT(int streamIndex, int shift, int outputStream);

protected:

	long long chunkSize;
	int numStreams;

	Stream<int>** stream;

	int* output;
};

#endif
