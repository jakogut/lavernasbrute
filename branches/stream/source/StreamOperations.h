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

    streamOperations(long long chunkSize);
   ~streamOperations();

    void setChunkSize(int input);

    void writeStream(unsigned int* input);
	void writeStream(Stream<unsigned int>* input);
    void readStream(unsigned int* output);

   Stream<unsigned int>* getStreamPtr();

   void operator =(streamOperations ops);
   void operator =(Stream<unsigned int> input);

   streamOperations operator +(streamOperations ops);
   streamOperations operator +(int operand);
   streamOperations operator -(streamOperations ops);
   streamOperations operator /(streamOperations ops);
   streamOperations operator *(streamOperations ops);

   void operator +=(streamOperations ops);
   void operator +=(int operand);
   void operator -=(streamOperations ops);
   void operator /=(streamOperations ops);
   void operator *=(streamOperations ops);

   streamOperations operator ~();
   streamOperations operator |(streamOperations ops);
   streamOperations operator ^(streamOperations ops);
   streamOperations operator &(streamOperations ops);

   streamOperations operator <<(int shift);
   streamOperations operator >>(int shift);

protected:

   long long chunkSize;

   Stream<unsigned int>* stream;
   Stream<unsigned int>* result;
   int* streamSize;
};

#endif