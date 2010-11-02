#ifndef MD4_COMMON_H_
#define MD4_COMMON_H_

#define SQRT_2 0x5a827999
#define SQRT_3 0x6ed9eba1

#define itoa16 "0123456789abcdef"

static unsigned int wd_init[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

typedef struct ctx
{
		unsigned int wd[4];

		union
		{
			unsigned int uint32[16];
			unsigned char uint8[64];
		} message;

} hashContext;

#endif
