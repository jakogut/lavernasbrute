//Part of Laverna's Brute

#include "ProcessingPath.h"

////////////////////////////////////////////
// Initialize our static variables /////////
////////////////////////////////////////////

boost::unordered_map<int64_pair, std::string> processingPath::targets;
boost::mutex processingPath::targetsMutex;
int processingPath::maxChars = 0;
int processingPath::numWorkers = 0;

////////////////////////////////////////////

processingPath::processingPath() 
{
	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	//Make sure the keyspace size is within the supported limits
	if(maxChars < 1 || maxChars > 14)
	{
		maxChars = 8;
	}

	numWorkers = masterThread::getNumWorkers();
}

processingPath::~processingPath()
{
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
	NTLM ntlm;
	targets[ntlm.weakenHash(input)] = input;
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

void processingPath::removeTarget(boost::unordered_map<int64_pair, std::string>::iterator it)
{
	boost::mutex::scoped_lock lock(targetsMutex);
	targets.erase(it);
}

////////////////////////
// Sequential Key //////
////////////////////////

keyGenerator::keyGenerator(unsigned long long input)
{
	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	key.reserve(processingPath::getMaxChars());

	location = input;
	integerToKey();
}

keyGenerator::keyGenerator(unsigned long long input, char* charset)
: charset(charset)
{
	charsetLength = strlen(charset);

	location = input;
	integerToKey();
}

keyGenerator::~keyGenerator()
{
}

void keyGenerator::incrementKey()
{
	for(unsigned int place = 0; place < key.length(); place++)
	{
		if(key[place] == charset[charset.length() - 1])
		{
			// Overflow, reset char at place
			key[place] = charset[0];
			keyIndices[place] = 0;

			if((key.length() - 1) < (place + 1))
			{
				// Carry, no space, insert char
				key.insert(key.begin(), charset[0]);
				keyIndices.insert(keyIndices.begin(), 0);
				break;
			}
			else
			{
				continue;
			}
		}
		else
		{
			// Space available, increment char at place
			key[place] = charset[keyIndices[place] + 1];
			keyIndices[place] = keyIndices[place] + 1;
			break;
		}
	}
}

std::string keyGenerator::operator++()
{
	incrementKey();
	return getKey();
}

std::string keyGenerator::operator++(int)
{
	std::string retval = key;
	incrementKey();

	return retval;
}

std::string keyGenerator::getKey()
{
	return key;
}

void keyGenerator::integerToKey()
{
	unsigned long long num = location;
	unsigned int remainder;

	key.clear();

	do
	{
		remainder = num % charsetLength;

		key.insert(key.begin(), charset[remainder]);
		keyIndices.insert(keyIndices.begin(), remainder);

	} while(num /= charsetLength);
}

unsigned long long keyGenerator::keyToInteger()
{
	// TODO
	return 0;
}