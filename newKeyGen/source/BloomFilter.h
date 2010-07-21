// Part of Laverna's Brute

#ifndef BLOOMFILTER_H_
#define BLOOMFILTER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	size_t filterSize;
	unsigned int* filter;
	
	unsigned int numBuckets;
	
} bloomFilter;

bloomFilter* bloomCreate(size_t filter);
void bloomDestroy(bloomFilter* bFilter);

void bloomAdd(const bloomFilter* bFilter, const unsigned int* input);

// Checks the bloom filter for potential matches; may return a false positive.
int bloomCheck(const bloomFilter* bFilter, const unsigned int* input);

#ifdef __cplusplus
}
#endif

#endif