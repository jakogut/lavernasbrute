//Part of Laverna's Brute

#include <iostream>
#include <time.h>
#include <string>

//Boost thread
#include <boost/thread/thread.hpp>

// Director class 
#include "Director.h"

//Base class for processing paths
#include "ProcessingPath.h"

//Got to have a slave driver
#include "MasterThread.h"

//Processing paths (Slaves)
#include "CPUPath.h"

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

	"\n\nntlm:HASH\tPush an NTLM hash onto the target list."
	"\n\t\tMultiple hashes can be attacked at once."

	"\n\n-t INTEGER\tNumber of CPU worker threads used."
	"\n\t\tThis should match the core count of your CPU."

	"\n\n-c INTEGER\tNumber of characters to include in the keyspace being searched."
	"\n\t\tMax for 32-bit is 8 chars, max for 64-bit is 14 chars."

	"\n\n-i INTEGER\tInterval in seconds for iterations logged to the console."
	"\n\t\tThe interval may be raised for a slight performance gain."

	"\n\n--silent\tRun the program in silent mode."

	"\n\n--frequency-charset\n\t\tUse a character set sorted by letter frequency."

	"\n\n--randomize-charset\n\t\tRandomizes the character set in order to prevent"
	"\n\t\tprediction of the keyspace search order."

	"\n\n--large-lookup\tUses a larger lookup table to improve speed."
	"\n\t\tIt is recommended that you have at least 1 GB memory"
	"\n\t\tin order to use this option."

	"\n\n--disable-lookup\n\t\tDisable the character lookup table."
	"\n\t\tThis may improve performance depending on the specifications" 
	"\n\t\tof your computer. This option also drastically decreases the"
	"\n\t\tamount of memory needed to run Laverna's Brute."

	"\n\n--disable-threading\n\t\tDisables threading -- This is not recommended.\n\n";
}

//Character array to integer (atoi) alternative, standards compliant.
int toInt(string input)
{
	stringstream ss;
	int result;

	ss << input;
	ss >> result;

	return result;
}

//Convert a string to lowercase
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

//Validates a string input as a lowercase hex digest of an NTLM hash
bool isValidNTLMHexDigest(const string hash)
{
    //Require a length of 32
    if (32 != hash.length()) return false;

    //Allow only hexidecimal characters
    for (unsigned int i = 0; i < hash.length(); ++i)
    {
        if (!(hash[i] >= '0' && hash[i] <= '9') &&
            !(hash[i] >= 'a' && hash[i] <= 'f'))
        {
            return false;
        }
    }

    //Passed validation
    return true;
}

int main(int argc, char** argv)
{
	bool targetPresent = false;
	int CPUThreads = 2;
	int numWorkers = 0;

	//Parse command-line arguments
	string flag, value;

	for(int i = 0; i < argc; i++)
	{
		flag = argv[i];

		if((i + 1) < argc)
			value = argv[i + 1];

		if(flag == "@TEST_MULTI")
		{
			string charset = " 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
			int charsetLength = (int)charset.length();

			int count = toInt(value);

			NTLM ntlm;

			for(int i = 0; i < count; i++)
			{
				unsigned long long num = 500000000 + i;
				string target = "";

				while(num)
				{
					target += charset[num % charsetLength];
					num /= charsetLength;
				}

				processingPath::pushTarget(ntlm.getNTLMHash(target));
			}

			targetPresent = true;
		}

		//Set the password string to be cracked
		if(flag == "-h" || flag == "--help")
		{
			printHelp();
			return 0;
		}

		//Take an NTLM hash and crack it
		if(flag.substr(0, 5) == "ntlm:")
		{
			string newHash = toLower(flag.substr(5));
 
			//Check to see whether the hash has been entered correctly
			//The length of a proper NTLM hash is always 32 characters
			if (isValidNTLMHexDigest(newHash))
			{
				processingPath::pushTarget((char*)newHash.c_str());
				targetPresent = true;
			}
			else
			{
				cerr << "\nERROR: \"" << newHash << "\" is an invalid hash! "
						"String must be a hex digest 32 characters in length." << endl;
			}
		}

		// Take a string, hash it, and crack it (for debugging purposes)
		if(flag.substr(0, 4) == "str:")
		{
			NTLM ntlm;
			string newHash = ntlm.getNTLMHash((char*)flag.substr(4).c_str());

			processingPath::pushTarget((char*)newHash.c_str());
			targetPresent = true;
		}

		//Set the number of threads
		if(flag == "-t")
		{
			if(toInt(value) >= 2)
			{
				CPUThreads = toInt(value);
			}
			else
			{
				CPUThreads = 2;
			}
		}

		if(flag == "-c")
		{
			processingPath::setMaxChars(toInt(value));
		}

		//Interval for iteration logging
		if(flag == "-i")
		{
			masterThread::setInterval(toInt(value));
		}		

		//Disable iteration logging
		if(flag == "--silent")
		{
			masterThread::setSilent(true);
		}

		// Use a character set sorted by letter frequency
		if(flag == "--frequency-charset")
		{
			masterThread::setFrequencyCharset(true);
		}

		/* Randomizing the order of the charset makes it impossible to predict the order in which the keyspace will be searched,
		This means that no string is better protected from cracking than any other string by its contents alone.
		With this option enabled, length is the only sure way to increase cracking times. */
		if(flag == "--randomize-charset")
		{
			masterThread::setRandomizeCharset(true);
		}

		if(flag == "--large-lookup")
		{
			masterThread::setLargeLookup(true);
		}

		if(flag == "--disable-lookup")
		{
			masterThread::disableLookup(true);
		}

		//Disable threading
		if(flag == "--disable-threading")
		{
			CPUThreads = 1;
		}
	}

	//Set the total number of worker threads in use for this run
	numWorkers += CPUThreads;
	masterThread::setNumWorkers(numWorkers);

	//Check to see that we have a valid target
	if(targetPresent)
	{
		cout << "\nRunning " << numWorkers << " (+1) cooperative threads," << endl
			 << "Cracking " << processingPath::getNumTargets()  << " hash(es).\n\n";
	}
	else
	{
		cerr << "\nERROR: Invalid input. Please choose a valid action." << endl
			 << "Use flag '-h' to see the possible options." << endl;

		return 1;
	}

	boost::thread_group threadGroup;
    
    // Create a director thread
	threadGroup.create_thread(Director());

	//Add a master thread to the group
	threadGroup.create_thread(masterThread());

	//Create a number of threads for the CPU path
	for(int i = 0; i < CPUThreads; i++)
	{
		threadGroup.create_thread(CPUPath(i));
	}

	threadGroup.join_all();

	return 0;
}
