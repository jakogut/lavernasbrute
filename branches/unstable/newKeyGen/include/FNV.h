// Part of Laverna's Brute

#ifndef FNV_H_
#define FNV_H_

#include <string.h>

#define FNV_Prime_32 0x100193
#define FNV_Offset_32 0x811C9DC5

inline unsigned int FNV32(const char* input, size_t size)
{
	unsigned int h = FNV_Offset_32, i;
	
	for(i = 0; i < size; i++)
	{
		h ^= (input[i]);
		h *= FNV_Prime_32;
	}
	
	return h;
}

#endif