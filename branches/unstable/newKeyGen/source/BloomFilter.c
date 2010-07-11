// Part of Laverna's Brute

#include <stdio.h>
#include <stdlib.h>

#include "bloomFilter.h"
#include "FNV.h"

#define GETBIT(a, n) (a[n / 8] & (1 << (n % 8)))
#define SETBIT(a, n) (a[n / 8] = a[n / 8] | 1 << (n % 8))

bloomFilter* bloomCreate(size_t filter,  size_t inputs)
{
	bloomFilter* bFilter;
	bFilter = (bloomFilter*)malloc(sizeof(bloomFilter));

	bFilter->filterSize = filter;
	bFilter->inputSize = inputs;
	
	bFilter->numBuckets = bFilter->filterSize * 8;

	bFilter->filter = (char*)malloc(bFilter->filterSize * sizeof(char));

	return bFilter;
}

void bloomDestroy(bloomFilter* bFilter)
{
	if(bFilter->filter) free(bFilter->filter);
	if(bFilter) free(bFilter);

	bFilter->filter = NULL;
	bFilter = NULL;
}

int bloomAdd(bloomFilter* bFilter, const void* input)
{
	SETBIT(bFilter->filter, hash(input, bFilter->inputSize, bFilter->numBuckets));
}


int bloomCheck(bloomFilter* bFilter, const void* input)
{
	if(GETBIT(bFilter->filter, hash(input, bFilter->inputSize, bFilter->numBuckets))) return 1;
	return 0;
}

unsigned int hash(const void* input, size_t size, unsigned int max)
{
	return FNV32((const char*)input, size) % max;
}
