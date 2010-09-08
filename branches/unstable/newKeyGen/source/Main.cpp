//Part of Laverna's Brute

#include <iostream>
#include <time.h>
#include <string>

#include <boost/thread/thread.hpp>

//Base class for processing paths
#include "ProcessingPath.h"

// The master thread helps organize the processing paths
#include "MasterThread.h"

// Processing paths
#include "CPUPath.h"

// Build and version configuration
#include "Version.h"

#ifdef SSE
#include "SSEPath.h"
#endif

using namespace std;

void printHelp()
{
	cout << "\nLaverna's Brute " << VERSION <<
	"\nThis program is a brute-force password cracker."
	"\nCOPYRIGHT " << COPYRIGHT << ", this program is licensed under the GNU GPL V3 "
	"\nSee \"http://lavernasbrute.googlecode.com/\" for more details. "
	"\n\nI am NOT responsible for loss of or damage to personal property as a result of "
	"\nthe use of this program. Any use of this program is at your own risk.\n"

	"\n\n[Flag] [Input]\tUsage."

	"\n\n-h\t\tDisplay this help message. "

	"\n\ntarget:HASH\tAdd an NTLM hash to the target list."
	"\n\t\tMultiple hashes can be attacked at once."

	"\n\n--hash-type\tSpecify the hashing algorithm to use."
	"\n\t\tAvailable options are: NTLM, MD4"

	"\n\n--filter-size\tSet the size of the bloom filter in bytes."
	"\n\t\tThe more target hashes, the larger the filter should be."
	"\n\t\tTry a filter size of 0.5*N, where N is the number of hashes."

	"\n\n-t INTEGER\tNumber of CPU worker threads used."
	"\n\t\tThis should match the core count of your CPU."

	"\n\n--charset STRING"
	"\n\t\tAvailable options are:"
	"\n\t\tloweralpha"
	"\n\t\tupperalpha"
	"\n\t\tmixalpha"
	"\n\t\tnumeric"
	"\n\t\tloweralpha-numeric"
	"\n\t\tupperalpha-numeric"
	"\n\t\tmixalpha-numeric"
	"\n\t\tall-chars"

	"\n\n-c INTEGER\tNumber of characters to include in the keyspace being searched."
	"\n\t\tMax is 10 chars."

#ifdef SSE
	"\n\n--SSE\t\tUse an SSE optimized CPU path."
#endif

	"\n\n--silent\tRun the program in silent mode."

	"\n\n--disable-threading\n\t\tDisables threading -- This is not recommended.\n\n";
}

// Portable character array to integer (atoi) alternative.
int toInt(string input)
{
	stringstream ss;
	int result;

	ss << input;
	ss >> result;

	return result;
}

/* Convert a string to a different case. 
 * If strCase is false, the string is converted to lowercase.
 * If strCase is true, the string is converted to uppercase.
 */

/* We use the same function to convert an entire string to upper or lowercase.
 * The boolean strCase dictates what case the string is converted to.
 * True == upper
 * False == lower */
string changeCase(string str, bool strCase)
{
	for(unsigned int i = 0; i < str.length(); i++)
	{
		if(strCase && (str[i] >= 'a' && str[i] <= 'z')) str[i] -= 32;
		else if(str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;
	}

	return str;
}

// Validates a string input as a lowercase hex digest
bool isValidHexDigest(const string hash)
{
    // Require a length of 32
    if (32 != hash.length()) return false;

    // Allow only hexidecimal characters
    for (unsigned int i = 0; i < hash.length(); ++i)
    {
        if (!(hash[i] >= '0' && hash[i] <= '9') &&
            !(hash[i] >= 'a' && hash[i] <= 'f'))
        {
            return false;
        }
    }

    // Passed validation
    return true;
}

int main(int argc, char** argv)
{
	bool hashTypeSpecified = false;
	bool targetPresent = false;
	bool charsetSpecified = false;

	std::string hashType;
	size_t filterSize = 0;

	#ifdef SSE
	bool enableSSE = false;
	#endif

	int CPUThreads = 2;

	processingPath::createBloomFilter();

	// Parse command-line arguments
	string flag, value;

	for(int i = 0; i < argc; i++)
	{
		flag = argv[i];

		if((i + 1) < argc)
			value = argv[i + 1];


#ifdef DEBUG
		// Test X number of hashes
		if(changeCase(flag, 0) == "--test")
		{
			unsigned int numHashes = toInt(value);

			MD4 md4;
			characterSet charset('a', 'z', 0, 0, 0, 0);
			keyGenerator keygen(500000, &charset);

			std::string newHash = md4.getHash_NTLM(keygen++);

			for(unsigned int i = 0; i < numHashes; i++)
				processingPath::addTarget(newHash);

			targetPresent = true;

		}
#endif

		// Print the help page
		if(changeCase(flag, 0) == "-h" || flag == "--help")
		{
			printHelp();
			return 0;
		}

		if(changeCase(flag, 0) == "--hash-type")
		{
			hashType = changeCase(value, 1);

			// This should be replaced with a function table sometime down the road.
			// Maybe after we add MD5 support. It's a low priority right now.
			if(hashType == "NTLM" || hashType == "MD4")
			{
				processingPath::setHashType(hashType);
				hashTypeSpecified = true;
			}
		}

		if(changeCase(flag, 0) == "--filter-size")
		{
			filterSize = toInt(value);
			processingPath::setBloomFilterSize(filterSize);
		}

		// Add a hash to the target hash map
		if(changeCase(flag.substr(0, 7), 0) == "target:")
		{
			string newHash = changeCase(flag.substr(7), 0);
 
			// Check to see whether the hash has been entered correctly
			// The length of a proper NTLM hash is always 32 characters
			if (isValidHexDigest(newHash))
			{
				processingPath::addTarget(newHash);
				targetPresent = true;
			}
			else
			{
				cerr << "\nERROR: \"" << newHash << "\" is an invalid hash!"
						"\nString must be a hex digest 32 characters in length." << endl;
			}
		}

		// Set the number of threads
		if(flag == "-t")
		{
			// If the specified number of threads is valid, use it. Otherwise, set it to two.
			(toInt(value) >= 2) ? CPUThreads = toInt(value) : CPUThreads = 2;
		}

		if(flag == "--charset")
		{
			charsetSpecified = true;

			if(value == "loweralpha")				// a-z
				masterThread::initCharset('a', 'z', 0, 0, 0, 0);
			else if(value == "upperalpha")			// A-Z
				masterThread::initCharset('A', 'Z', 0, 0, 0, 0);
			else if(value == "mixalpha")			// a-z, A-Z
				masterThread::initCharset('A', 'z', 'Z'+1, 'a', 0, 0);
			else if(value == "numeric")				// 0-9
				masterThread::initCharset('0', '9', 0, 0, 0, 0);
			else if(value == "loweralpha-numeric")	// a-z, 0-9
				masterThread::initCharset('0', 'z', '9'+1, 'a', 0, 0);
			else if(value == "upperalpha-numeric")	// A-Z, 0-9
				masterThread::initCharset('0', 'Z', '9'+1, 'A', 0, 0);
			else if(value == "mixalpha-numeric")	// A-Z, a-z, 0-9
				masterThread::initCharset('0', 'z', '9'+1, 'A', 'Z'+1, 'a');
			else									// All ascii symbols and characters
				masterThread::initCharset(' ', '~', 0, 0, 0, 0);
		}

		// Set the maximum size of the keyspace in characters. The number of permutations is pow(charsetLength, maxChars).
		if(flag == "-c")
		{
			processingPath::setMaxChars(toInt(value));
		}

		// Enable SSE path
		if(changeCase(flag, 0) == "--sse" || changeCase(flag, 0) == "--sse2")
		{
			#ifdef SSE
			enableSSE = true;
			#else
			cerr << "Warning: SSE is not supported in this build." << endl;
			#endif
		}

		// Disable iteration logging
		if(changeCase(flag, 0) == "--silent")
		{
			masterThread::setSilent(true);
		}

		// Disable threading
		if(changeCase(flag, 0) == "--disable-threading")
		{
			CPUThreads = 1;
		}
	}

	if(!charsetSpecified)
		masterThread::initCharset('0', 'z', '9'+1, 'A', 'Z'+1, 'a');

	// Check to see that we have a valid target
	if(targetPresent && hashTypeSpecified)
	{
		cout << "\nRunning " << CPUThreads << " cooperative threads," << endl
			 << "Cracking " << processingPath::getNumTargets()  << " " << hashType << " hash(es)." << endl << endl;

		if(filterSize)
			cout << "Filter size of " << filterSize << " bytes specified." << endl << endl;

		#ifdef SSE
		if(enableSSE)
		{
			cout << "SSE processing path enabled." << endl << endl;
		}
		else
		#endif
			cout << endl << endl;
	}
	else
	{
		if(!targetPresent)
			cerr << "\nERROR: Invalid input. Please choose a valid action." << endl;

		if(!hashTypeSpecified)
			cerr << "\nERROR: No hash type specified. Please specify a valid hash type." << endl;

		cout << "\nUse flag '-h' to see the possible options." << endl;

		return 1;
	}

	// Create a thread group
	boost::thread_group threadGroup;

	// Create the master thread
	threadGroup.create_thread(masterThread());

	// Create the appropriate number of threads for the CPU path
	for(int i = 0; i < CPUThreads; i++)
	{
		#ifdef SSE
		if(enableSSE)
		{
			threadGroup.create_thread(SSEPath(i));
		}
		else
		#endif
			threadGroup.create_thread(CPUPath(i));
	}

	// Wait for the threads to complete their work
	threadGroup.join_all();

	// Destroy the bloom filter
	processingPath::destroyBloomFilter();

	return 0;
}
