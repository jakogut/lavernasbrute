// Part of Laverna's Brute

#ifndef BLOOMFILTER_H_
#define BLOOMFILTER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Bloom filters are used to verify that a given key is present in a dataset before searching the dataset.
Laverna's Brute uses this to improve performance by skipping unnecessary searches of the target list 
when we know that the key is not present. */

typedef unsigned int (*hashFunc)(const char*);

typedef struct
{
	size_t arraySize;
	unsigned char* bitArray;

	size_t numHashFunctions;
	hashFunc* hashFunctions;


} bloomFilter;

bloomFilter* bloomCreate(size_t size, size_t nFuncs, ...);
void bloomDestroy(bloomFilter* bloom);

void bloomAdd(bloomFilter* bloom, const void* input);
int bloomCheck(bloomFilter* bloom, const void* input);

// Hashing function(s)
unsigned int sax_hash(const char* key);
unsigned int sdbm_hash(const char* key);

#ifdef __cplusplus
}
#endif

#endif