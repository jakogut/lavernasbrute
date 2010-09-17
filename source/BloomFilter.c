// Part of Laverna's Brute

#include <stdio.h>
#include <stdlib.h>

#include "BloomFilter.h"

#define GETBIT(a, n) (a[n >> 5] & (1 << (n & 31)))
#define SETBIT(a, n) (a[n >> 5] = (a[n >> 5] | 1 << (n & 31)))

bloomFilter* bloomCreate(size_t filter)
{
	bloomFilter* bFilter;
	bFilter = (bloomFilter*)malloc(sizeof(bloomFilter));

	bFilter->filterSize = filter;
	
	bFilter->numBuckets = (bFilter->filterSize + 3) * 32;

	bFilter->filter = (unsigned*)malloc((bFilter->filterSize + 3) * sizeof(unsigned));

	return bFilter;
}

void bloomDestroy(bloomFilter* bFilter)
{
	if(bFilter->filter) free(bFilter->filter);
	if(bFilter) free(bFilter);

	bFilter->filter = NULL;
	bFilter = NULL;
}

void bloomAdd(const bloomFilter* bFilter, const unsigned int* input)
{
	SETBIT(bFilter->filter, *input & (bFilter->numBuckets - 1));
}


int bloomCheck(const bloomFilter* bFilter, const unsigned int* input)
{
	if(GETBIT(bFilter->filter, *input &  (bFilter->numBuckets - 1))) return 1;
	return 0;
}