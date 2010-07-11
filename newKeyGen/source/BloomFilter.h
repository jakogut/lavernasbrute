// Part of Laverna's Brute

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	unsigned int wd[4];
} hashContext;

typedef struct
{
	size_t filterSize;
	char* filter;
	
	unsigned int numBuckets;
	
	// We need to know how big the input objects are in bytes
	size_t inputSize;
	
} bloomFilter;

bloomFilter* bloomCreate(size_t filter, size_t inputs);
void bloomDestroy(bloomFilter* bFilter);

int bloomAdd(bloomFilter* bFilter, const void* input);

// If positive, return one, else return zero.
int bloomCheck(bloomFilter* bFilter, const void* input);

inline unsigned int hash(const void* input, size_t size, unsigned int max);

#ifdef __cplusplus
}
#endif
