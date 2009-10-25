//Part of Laverna's Brute

#include "ProcessingPath.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

boost::unordered_map<unsigned int, std::string> processingPath::targets;
int processingPath::maxChars = 0;
int processingPath::numWorkers = 0;

////////////////////////////////////////////

processingPath::processingPath() 
{
	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	integerToKeyLookup = masterThread::getLookup();

	//Make sure the keyspace size is within the supported limits
	if(maxChars < 1 || maxChars > 14)
	{
		maxChars = 8;
	}

	numWorkers = masterThread::getNumWorkers();
}

processingPath::~processingPath()
{
	integerToKeyLookup = NULL;
}

void processingPath::operator ()()
{
}

int processingPath::getThreadID()
{
	return -1;
}

// These are all implementation specific
unsigned long long processingPath::getKeyspaceEnd()
{
	return -1;
}

unsigned long long processingPath::getKeyspaceBegin()
{
	return -1;
}

unsigned long long processingPath::getKeyLocation()
{
	return -1;
}

void processingPath::moveKeyspaceEnd(unsigned long long input)
{
}

void processingPath::moveKeyspaceBegin(unsigned long long input)
{

}
void processingPath::moveKeylocation(unsigned long long input)
{
}

void processingPath::pushTarget(std::string input)
{
	targets[hash(&input)] = input;
}

void processingPath::setMaxChars(int input)
{
	maxChars = input;
}

int processingPath::getNumTargets()
{
	return (int)targets.size();
}

unsigned int processingPath::hash(std::string* input)
{
	unsigned char* str = (unsigned char*)input->c_str();
	unsigned int hash = 0;
	unsigned int c;

	while(c = *str++)
		hash = (hash << 5) * (hash >> 3) + c;

	return hash;
}