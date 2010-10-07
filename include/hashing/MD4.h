/* Slightly modified for LB, orginal code written by Alain Espinosa <alainesp@gmail.com>. Thanks, Alain! */

#ifndef MD4_H_
#define MD4_H_

#include <cstring>
#include <iostream>
#include <cstdlib>

#include "MD4_common.h"

#define ROTL(NUM, PLACES) ((NUM << PLACES) | (NUM >> (32 - PLACES)))
#define ROTR(NUM, PLACES) ((NUM >> PLACES) | (NUM << (32 - PLACES)))

#define F(X, Y, Z) ((X & Y) | (~X & Z))
#define G(X, Y, Z) ((X & Y) | (X & Z) | (Y & Z))
#define H(X, Y, Z) (X ^ Y ^ Z)

#define FF(A, B, C, D, X, ROT) \
	A += F(B, C, D) + X, A = ROTL(A, ROT)

#define GG(A, B, C, D, X, SQRT2, ROT) \
	A += G(B, C, D) + X + SQRT2, A = ROTL(A, ROT)

#define HH(A, B, C, D, X, SQRT3, ROT) \
	A += H(B, C, D) + X + SQRT3, A = ROTL(A, ROT)

#define LOOP(CODE, N) for(int i = 0; i < N; i++) CODE

typedef struct ctx
{
		unsigned int wd[4];

		union
		{
			unsigned int uint32[16];
			unsigned char uint8[64];
		} message;

} hashContext;

class MD4
{
public:

	MD4()
	{
	}

	~MD4()
	{
	}

	typedef void (MD4::*scalarHashPtr)(hashContext*);

	// Full MD4
	inline char* getHash_MD4(const char* input)
	{
		hashContext ctx;

		// Take an ASCII string, and convert it to an MD4 message
		prepareKey_MD4(&ctx, input);

		initialize(&ctx, 1);
		encrypt(&ctx, 1);
		finalize(&ctx);

		return convertToHex(&ctx);
	}

	// Context only MD4
	inline void getHashContext_MD4(hashContext* ctx)
	{
		initialize(ctx, 1);
		encrypt(ctx, 1);
	}

	// Full NTLM
	inline char* getHash_NTLM(const char* input)
	{
		hashContext ctx;

		prepareKey_NTLM(&ctx, input);

		initialize(&ctx, 1);
		encrypt(&ctx, 1);
		finalize(&ctx);

		return convertToHex(&ctx);
	}

	// Context only NTLM
	inline void getHashContext_NTLM(hashContext* ctx)
	{
		getHashContext_NTLM(ctx, 1);
	}

	inline void getHashContext_NTLM(hashContext* ctx, unsigned int n)
	{
		initialize(ctx, n);
		encrypt(ctx, n);
	}

	// Take an MD4 hash as input, and reverse the hex encoding, returning a hashContext
	inline void hashToContext(hashContext* ctx, const char* input)
	{
		convertFromHex(ctx, input);
	}

	inline char* contextToHash(hashContext* ctx)
	{
		ctx->wd[0] += wd_init[0];
		ctx->wd[1] += wd_init[1];
		ctx->wd[2] += wd_init[2];
		ctx->wd[3] += wd_init[3];

		return convertToHex(ctx);
	}

protected:

	void prepareKey_MD4(hashContext* ctx, const char* input)
	{
		int i=0;
		int length = (int)strlen(input);

		unsigned int* message = ctx->message.uint32;

		// Zero out the message buffer
		memset(message,0,16*4);

		for(;i<length/4;i++)
			message[i] = input[4*i] | (input[4*i+1]<<8) | (input[4*i+2]<<16) | (input[4*i+3]<<24);

		// Pad with one 1 bit, followed by zeros until the message is 64 bits shy of 512 bits in length.
		switch(length%4)
		{
		case 0:
			message[i] = 0x80;
			break;
		case 1:
			message[i] = input[length - 1] | 0x8000;
			break;
		case 2:
			message[i] = input[length-2] | (input[length-1]<<8) | 0x800000;
			break;
		case 3:
			message[i] = input[length-3] | (input[length-2]<<8) | (input[length-1]<<16) | 0x80000000;
			break;
		}

		// Add the length
		message[14] = length << 3;
	}

	void prepareKey_NTLM(hashContext* ctx, const char* input)
	{
		int i=0;
		int length=(int)(strlen(input));

		unsigned int* message = ctx->message.uint32;

		memset(message,0,16*4);

		for(;i<length/2;i++)
			message[i] = input[2*i] | (input[2*i+1]<<16);

		// Pad
		if(length%2==1)
			message[i] = input[length-1] | 0x800000;
		else
			message[i]=0x80;
		// Length
		message[14] = length << 4;
	}

	inline virtual void initialize(hashContext* ctx, int n)
	{
		for(int i = 0; i < n; i++) memcpy(ctx->wd, wd_init, 4 * sizeof(unsigned));
	}

	inline virtual void encrypt(hashContext* ctx, int n)
	{
		unsigned int* wd = ctx->wd;
		unsigned int* message = ctx->message.uint32;

		register unsigned int a(wd[0]), b(wd[1]), c(wd[2]), d(wd[3]);

		// Round 1 // ---

		LOOP(FF(a, b, c, d, message[0], 3), n);
		LOOP(FF(d, a, b, c, message[1], 7), n);
		LOOP(FF(c, d, a, b, message[2], 11), n);
		LOOP(FF(b, c, d, a, message[3], 19), n);

		LOOP(FF(a, b, c, d, message[4], 3), n);
		LOOP(FF(d, a, b, c, message[5], 7), n);
		LOOP(FF(c, d, a, b, message[6], 11), n);
		LOOP(FF(b, c, d, a, message[7], 19), n);

		LOOP(FF(a, b, c, d, message[8],  3), n);
		LOOP(FF(d, a, b, c, message[9],  7), n);
		LOOP(FF(c, d, a, b, message[10], 11), n);
		LOOP(FF(b, c, d, a, message[11], 19), n);

		LOOP(FF(a, b, c, d, message[12], 3), n);
		LOOP(FF(d, a, b, c, message[13], 7), n);
		LOOP(FF(c, d, a, b, message[14], 11), n);
		LOOP(FF(b, c, d, a, message[15], 19), n);

		// Round 2 // ---

		LOOP(GG(a, b, c, d, message[0],  SQRT_2, 3), n);
		LOOP(GG(d, a, b, c, message[4],  SQRT_2, 5), n);
		LOOP(GG(c, d, a, b, message[8],  SQRT_2, 9), n);
		LOOP(GG(b, c, d, a, message[12], SQRT_2, 13), n);

		LOOP(GG(a, b, c, d, message[1],  SQRT_2, 3), n);
		LOOP(GG(d, a, b, c, message[5],  SQRT_2, 5), n);
		LOOP(GG(c, d, a, b, message[9],  SQRT_2, 9), n);
		LOOP(GG(b, c, d, a, message[13], SQRT_2, 13), n);

		LOOP(GG(a, b, c, d, message[2],  SQRT_2, 3), n);
		LOOP(GG(d, a, b, c, message[6],  SQRT_2, 5), n);
		LOOP(GG(c, d, a, b, message[10], SQRT_2, 9), n);
		LOOP(GG(b, c, d, a, message[14], SQRT_2, 13), n);

		LOOP(GG(a, b, c, d, message[3],  SQRT_2, 3), n);
		LOOP(GG(d, a, b, c, message[7],  SQRT_2, 5), n);
		LOOP(GG(c, d, a, b, message[11], SQRT_2, 9), n);
		LOOP(GG(b, c, d, a, message[15], SQRT_2, 13), n);

		// Round 3 // ---

		LOOP(HH(a, d, c, b, message[0],  SQRT_3, 3), n);
		LOOP(HH(d, c, b, a, message[8],  SQRT_3, 9), n);
		LOOP(HH(c, b, a, d, message[4],  SQRT_3, 11), n);
		LOOP(HH(b, a, d, c, message[12], SQRT_3, 15), n);

		LOOP(HH(a, d, c, b, message[2],  SQRT_3, 3), n);
		LOOP(HH(d, c, b, a, message[10], SQRT_3, 9), n);
		LOOP(HH(c, b, a, d, message[6],  SQRT_3, 11), n);
		LOOP(HH(b, a, d, c, message[14], SQRT_3, 15), n);

		LOOP(HH(a, d, c, b, message[1],  SQRT_3, 3), n);
		LOOP(HH(d, c, b, a, message[9],  SQRT_3, 9), n);
		LOOP(HH(c, b, a, d, message[5],  SQRT_3, 11), n);
		LOOP(HH(b, a, d, c, message[13], SQRT_3, 15), n);

		LOOP(HH(a, d, c, b, message[3],  SQRT_3, 3), n);
		LOOP(HH(d, c, b, a, message[11], SQRT_3, 9), n);
		LOOP(HH(c, b, a, d, message[7],  SQRT_3, 11), n);
		LOOP(HH(b, a, d, c, message[15], SQRT_3, 15), n);

		ctx->wd[0] = a;
		ctx->wd[1] = b;
		ctx->wd[2] = c;
		ctx->wd[3] = d;
	}

	inline virtual void finalize(hashContext* ctx)
	{
		ctx->wd[0] += wd_init[0];
		ctx->wd[1] += wd_init[1];
		ctx->wd[2] += wd_init[2];
		ctx->wd[3] += wd_init[3];
	}

	char* convertToHex(hashContext* ctx)
	{
		//Iterate the integer
		for(int i = 0; i < 4; i++)
		{
			unsigned int n = ctx->wd[i];
			//iterate the bytes of the integer
			for(int j = 0; j < 4; j++)
			{
				unsigned int convert = n % 256;
				hex_format[i*8+j*2+1]=itoa16[convert%16];
				convert /= 16;
				hex_format[i*8+j*2+0]=itoa16[convert%16];
				n /= 256;
			}
		}
		//null terminate the string
		hex_format[32] = 0;

		return hex_format;
	}

	// Convert a hex digest back to four 32-bit words
	void convertFromHex(hashContext* ctx, const char* hash)
	{
		char bigEndianHash[4][9];

		for(int i = 0; i < 4; i++)
		{
			// This needs to be reworked
			bigEndianHash[i][0] = hash[(i*8)+6];
			bigEndianHash[i][1] = hash[(i*8)+7];
			bigEndianHash[i][2] = hash[(i*8)+4];
			bigEndianHash[i][3] = hash[(i*8)+5];
			bigEndianHash[i][4] = hash[(i*8)+2];
			bigEndianHash[i][5] = hash[(i*8)+3];
			bigEndianHash[i][6] = hash[(i*8)+0];
			bigEndianHash[i][7] = hash[(i*8)+1];

			bigEndianHash[i][8] = '\0';
		}

		for(int i = 0; i < 4; i++)
		{
			ctx->wd[i] = strtoul(bigEndianHash[i], 0, 16);
		}

		ctx->wd[0] -= wd_init[0];
		ctx->wd[1] -= wd_init[1];
		ctx->wd[2] -= wd_init[2];
		ctx->wd[3] -= wd_init[3];
	}

private:

	hashContext retval;
	char hex_format[33]; // Char array used to store the resulting hex digest
};

#endif

