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

	void writeStream(int stream, int input[]);
	int* readResult();

	void ADD(int input1);
	inline void ADD();
	inline void SUB();
	inline void DIV();
	inline void MUL();

	inline void NOT();
	inline void OR();
	inline void XOR_BI();
	inline void XOR_TRI();
	inline void AND();

	inline void LSHIFT(int shift);
	inline void RSHIFT(int shift);

	void setChunkSize(int input);

protected:

	int chunkSize;

	Stream<int>* inputStream0;
	Stream<int>* inputStream1;
	Stream<int>* inputStream2;

	Stream<int>* outputStream;
	int* output;
};

#endif
