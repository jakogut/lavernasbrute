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

sequentialKey::sequentialKey(unsigned long long input)
{
	charset = masterThread::getCharset();
	charsetLength = masterThread::getCharsetLength();

	key.reserve(processingPath::getMaxChars());

	location = input;
	integerToKey();
}

sequentialKey::~sequentialKey()
{
}

void sequentialKey::operator++()
{
	for(unsigned int place = 0; place < key.length(); place++)
	{
		if(key[place] == charset[charset.length() - 1])
		{
			// Overflow, reset char at place
			key[place] = charset[0];

			if((key.length() - 1) < (place + 1))
			{
				// Carry, no space, insert char
				key.insert(key.begin(), charset[0]);
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
			key[place] = charset[charset.find(key[place]) + 1];
			break;
		}
	}
}

void sequentialKey::operator++(int)
{
	this->operator++();
}

std::string sequentialKey::getKey()
{
	return key;
}

void sequentialKey::integerToKey()
{
	unsigned long long num = location;
	key.clear();

	do
	{
		key += charset[num % charsetLength];
	} while(num /= charsetLength);
}

unsigned long long sequentialKey::keyToInteger()
{
	// TODO
	return 0;
}