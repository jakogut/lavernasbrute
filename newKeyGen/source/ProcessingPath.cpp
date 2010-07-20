//Part of Laverna's Brute

#include "ProcessingPath.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

std::vector<hashContext> processingPath::targets;
int processingPath::maxChars = 0;
std::string processingPath::hashType;
bloomFilter* processingPath::bFilter;
size_t processingPath::bFilterSize = 32000;

////////////////////////////////////////////

processingPath::processingPath() 
{
	//Make sure the keyspace size is within the supported limits
	if(maxChars < 1 || maxChars > 10)
	{
		maxChars = 10;
	}

	// Sort the target array
	std::sort(targets.begin(), targets.end());
}

processingPath::~processingPath()
{
	//if(bFilter) bloomDestroy(bFilter);
}

void processingPath::initializeBloomFilter()
{
	bFilter = bloomCreate(bFilterSize);
}

void processingPath::setBloomFilterSize(size_t size)
{
	bFilterSize = size;
}

void processingPath::addTarget(std::string& input)
{
	MD4 md4;
	hashContext* ctx = md4.hashToContext(input.c_str());

	targets.push_back(*ctx);
	bloomAdd(bFilter, ctx->uint32[0]);
}

void processingPath::setMaxChars(int input)
{
	maxChars = input;
}

int processingPath::getMaxChars()
{
	return maxChars;
}

int processingPath::getNumTargets()
{
	return (int)targets.size();
}

void processingPath::setHashType(std::string type)
{
	hashType = type;
}

unsigned long long processingPath::calculateKeyspaceSize(int charsetLength, int keyLength)
{
	unsigned long long  temp, retval(1);

	for(int i = 0; i < keyLength; i++)
	{
		temp = 1;

		for(int j = 0; j < i; j++)
			temp *= (unsigned long long)charsetLength;

		retval += temp;
	}

	return retval;
}

unsigned long long processingPath::calculateKeyspaceSize()
{
	return calculateKeyspaceSize(masterThread::getCharset()->length, maxChars);
}

bool operator<(const hashContext& a, const hashContext& b)
{
	if(memcmp(b.uint32, a.uint32, 4 * sizeof(unsigned)) > 0) return true;
	else return false;
}