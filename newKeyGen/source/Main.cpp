//Part of Laverna's Brute

// Enable functions which aid in developing and debugging for Laverna's Brute.
// Comment this out for release builds.
//#define DEBUG

#include <iostream>
#include <time.h>
#include <string>

#include <boost/thread/thread.hpp>

// Director class
#include "Director.h"

//Base class for processing paths
#include "ProcessingPath.h"

// The master thread helps organize the processing paths
#include "MasterThread.h"

// Processing paths
#include "CPUPath.h"
#include "SSE2Path.h"
//#include "64BitPath.h"

using namespace std;

void printHelp()
{
	cout << "\nLaverna's Brute."
	"\nThis program is a brute-force password cracker."
	"\nCOPYRIGHT 2008, this program is licensed under the GNU GPL V3 "
	"\nSee \"http://lavernasbrute.googlecode.com/\" for more details. "
	"\n\nI am NOT responsible for loss of or damage to personal property as a result of "
	"\nthe use of this program. Any use of this program is at your own risk.\n"

	"\n\n[Flag] [Input]\tUsage."

	"\n\n-h\t\tDisplay this help message. "

	"\n\nntlm:HASH\tAdd an NTLM hash to the target list."
	"\n\t\tMultiple hashes can be attacked at once."

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

	"\n\n--SSE2\t\tUse an SSE2 optimized CPU path."

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

// Convert a string to lowercase
string toLower(const string input)
{
	string result = input;

	for(unsigned int i = 0; i < result.length(); i++)
	{
		if(result[i] >= 65 && result[i] <= 90)
		{
			result[i] += 32;
		}
	}

	return result;
}

// Validates a string input as a lowercase hex digest of an NTLM hash
bool isValidNTLMHexDigest(const string hash)
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
	bool targetPresent = false;
	bool enableSSE2 = false;
	int CPUThreads = 2;

	// Parse command-line arguments
	string flag, value;

	for(int i = 0; i < argc; i++)
	{
		flag = argv[i];

		if((i + 1) < argc)
			value = argv[i + 1];

		#ifdef DEBUG
		if(toLower(flag) == "--test_multi")
        {
                string charset = " 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
                int charsetLength = (int)charset.length();

                int count = toInt(value);

                NTLM ntlm;

                for(int i = 0; i < count; i++)
                {
                        unsigned long long num = 500000000 + i;
                        string target = "";

						num++;
                        while(num)
                        {
								num--;
                                target += charset[num % charsetLength];
                                num /= charsetLength;
                        }

                        processingPath::pushTarget(ntlm.getNTLMHash(target));
                }

                targetPresent = true;
        }
		#endif

		// Print the help page
		if(toLower(flag) == "-h" || flag == "--help")
		{
			printHelp();
			return 0;
		}

		// Add an NTLM hash to the targets hash map
		if(toLower(flag.substr(0, 5)) == "ntlm:")
		{
			string newHash = toLower(flag.substr(5));
 
			// Check to see whether the hash has been entered correctly
			// The length of a proper NTLM hash is always 32 characters
			if (isValidNTLMHexDigest(newHash))
			{
				processingPath::pushTarget((char*)newHash.c_str());
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
			if(value == "loweralpha")
				masterThread::initCharset(26, 'a', 'z', 0, 0, 0, 0);
			else if(value == "upperalpha")
				masterThread::initCharset(26, 'A', 'Z', 0, 0, 0, 0);
			else if(value == "mixalpha")
				masterThread::initCharset(52, 'A', 'z', 'Z'+1, 'a', 0, 0);
			else if(value == "numeric")
				masterThread::initCharset(10, '0', '9', 0, 0, 0, 0);
			else if(value == "loweralpha-numeric")
				masterThread::initCharset(36, '0', 'z', '9'+1, 'a', 0, 0);
			else if(value == "upperalpha-numeric")
				masterThread::initCharset(36, '0', 'Z', '9'+1, 'A', 0, 0);
			else if(value == "mixalpha-numeric")
				masterThread::initCharset(62, '0', 'z', '9'+1, 'A', 'Z'+1, 'a');
			else // All chars
				masterThread::initCharset(95, ' ', '~', 0, 0, 0, 0);
		}

		// Set the maximum size of the keyspace in characters. The number of permutations is pow(charsetLength, maxChars).
		if(flag == "-c")
		{
			processingPath::setMaxChars(toInt(value));
		}		

		// Enable SSE2 path
		if(toLower(flag) == "--sse2")
		{
			enableSSE2 = true;
		}

		// Disable iteration logging
		if(toLower(flag) == "--silent")
		{
			masterThread::setSilent(true);
		}

		// Disable threading
		if(toLower(flag) == "--disable-threading")
		{
			CPUThreads = 1;
		}
	}

	// Set the total number of worker threads in use for this run
	masterThread::setNumWorkers(CPUThreads);

	// Check to see that we have a valid target
	if(targetPresent)
	{
		cout << "\nRunning " << CPUThreads << " cooperative threads," << endl
			 << "Cracking " << processingPath::getNumTargets()  << " hash(es).";

		if(enableSSE2)
		{
			cout << "\n\nSSE2 processing path enabled." << endl << endl;
		}
		else
		{
			cout << endl << endl;
		}
	}
	else
	{
		cerr << "\nERROR: Invalid input. Please choose a valid action." << endl
			 << "Use flag '-h' to see the possible options." << endl;

		return 1;
	}

	// Create a thread group
	boost::thread_group threadGroup;

	// Create the master thread
	threadGroup.create_thread(masterThread());

	// Create the director thread
	threadGroup.create_thread(Director());

	// Create the appropriate number of threads for the CPU path
	for(int i = 0; i < CPUThreads; i++)
	{
		if(enableSSE2)
		{
			threadGroup.create_thread(SSE2Path(i));
		}
		else
		{
			threadGroup.create_thread(CPUPath(i));
		}
	}

	// Wait for the threads to complete their work
	threadGroup.join_all();

	return 0;
}
