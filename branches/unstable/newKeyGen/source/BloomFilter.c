// Part of Laverna's Brute

#include <stdio.h>
#include <stdlib.h>

#include "BloomFilter.h"

#define GETBIT(a, n) (a[n >> 3] & (1 << (n & 7)))
#define SETBIT(a, n) (a[n >> 3] = (a[n >> 3] | 1 << (n & 7)))

bloomFilter* bloomCreate(size_t filter)
{
	bloomFilter* bFilter;
	bFilter = (bloomFilter*)malloc(sizeof(bloomFilter));

	bFilter->filterSize = filter;
	
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

int bloomAdd(bloomFilter* bFilter, unsigned int input)
{
	if(!bFilter) return -1;

	SETBIT(bFilter->filter, ((input > bFilter->numBuckets) ? (input % bFilter->numBuckets) : input));
	return 1;
}


int bloomCheck(const bloomFilter* bFilter, unsigned int input)
{
	if(GETBIT(bFilter->filter, ((input > bFilter->numBuckets) ? (input % bFilter->numBuckets) : input))) return 1;
	return 0;
}