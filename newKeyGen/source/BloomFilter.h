// Part of Laverna's Brute

#ifndef BLOOMFILTER_H_
#define BLOOMFILTER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	size_t filterSize;
	char* filter;
	
	unsigned int numBuckets;
	
} bloomFilter;

bloomFilter* bloomCreate(size_t filter);
void bloomDestroy(bloomFilter* bFilter);

int bloomAdd(bloomFilter* bFilter, unsigned int input);

// Checks the bloom filter for potential matches; may return a false positive.
int bloomCheck(const bloomFilter* bFilter, unsigned int input);

#ifdef __cplusplus
}
#endif

#endif