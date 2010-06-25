// Part of Laverna's Brute

#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>

#include "BloomFilter.h"

#define NUM_HASH_FUNCS 2

#define SETBIT(a, n) (a[n/CHAR_BIT] |= (1<<(n%CHAR_BIT)))
#define GETBIT(a, n) (a[n/CHAR_BIT] & (1<<(n%CHAR_BIT)))

bloomFilter* bloomCreate(size_t size, size_t nFuncs, ...)
{
	bloomFilter* bloom;

	va_list list;
	int i;

	bloom = (bloomFilter*)malloc(sizeof(bloomFilter));
	bloom->bitArray = (unsigned char*)calloc((size + (CHAR_BIT - 1)) / CHAR_BIT, sizeof(char));

	bloom->hashFunctions = (hashFunc*)malloc(nFuncs * sizeof(hashFunc));

	va_start(list, nFuncs);
	for(i = 0; i < nFuncs; i++) bloom->hashFunctions[i] = va_arg(list, hashFunc);
	va_end(list);

	bloom->numHashFunctions = nFuncs;
	bloom->arraySize = size;

	return bloom;
}

void bloomDestroy(bloomFilter* bloom)
{
	free(bloom->bitArray);
	free(bloom);
}

void bloomAdd(bloomFilter* bloom, const void* input)
{
	const char* cstr = (const char*)input;

	int i = 0;
	for(; i < bloom->numHashFunctions; i++) 
		SETBIT(bloom->bitArray, bloom->hashFunctions[i](cstr) % bloom->arraySize);
}

int bloomCheck(bloomFilter* bloom, const void* input)
{
	const char* cstr = (const char*)input;

	int i = 0;
	for(; i < bloom->numHashFunctions; i++) 
		if(!GETBIT(bloom->bitArray, bloom->hashFunctions[i](input) % bloom->arraySize)) return 0;

	return 1;
}

unsigned int sax_hash(const char *key)
{
	unsigned int h=0;

	while(*key) h^=(h<<5)+(h>>2)+(unsigned char)*key++;

	return h;
}

unsigned int sdbm_hash(const char *key)
{
	unsigned int h=0;
	while(*key) h=(unsigned char)*key++ + (h<<6) + (h<<16) - h;
	return h;
}