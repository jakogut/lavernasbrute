#ifndef MD4_COMMON_H_
#define MD4_COMMON_H_

#define SQRT_2 0x5a827999
#define SQRT_3 0x6ed9eba1

#define itoa16 "0123456789abcdef"

static unsigned int wd_init[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

typedef struct ctx
{
		ctx()
		{
			messageLength = 0;
		}

		// Stores 4 32-bit words used in the encryption process
		unsigned int wd[4];

		// The input message stored in 16 32-bit words, or 64 8-bit words
		// (the former is faster, while the latter format makes modification of individual characters easier)
		union
		{
			unsigned int uint32[16];
			unsigned char uint8[64];
		} message;

		// The message length is stored in the context, because memory is cheap, and processing power is expensive.
		int messageLength;

} hashContext;

#endif
