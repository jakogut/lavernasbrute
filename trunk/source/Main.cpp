//Part of Laverna's Brute

#include <iostream>
#include <time.h>
#include <string>

//Boost thread
#include <boost/thread/thread.hpp>

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

	"\n\n-b HASH\t\tTake an NTLM hash and brute force it."

	"\n\n-s STRING\tGenerate an NTLM hash from a text string and brute force it."

	"\n\n-t INTEGER\tNumber of CPU worker threads used."
	"\n\t\tThis should match the core count of your CPU."

	"\n\n-c INTEGER\tNumber of characters to include in the keyspace being searched."

	"\n\n-i INTEGER\tInterval in seconds for iterations logged to the console."
	"\n\t\tThe interval may be raised for a slight performance gain."

	"\n\n--silent\tRun the program in silent mode."

	"\n\n--randomize-charset\n\t\tRandomizes the character set in order to prevent"
	"\n\t\tprediction of the keyspace search order."

	"\n\n--large-lookup\tUses a larger lookup table to improve speed."
	"\n\t\tIt is recommended that you have at least 350mb of free memory"
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
		if(result[i] >= 0x41 && result[i] <= 0x5A)
		{
			result[i] += 0x20;
		}
	}

	return result;
}

int main(int argc, char** argv)
{
	bool silent = false;
	string hashTemp;
	int interval = 5;

	//Number of threads for the CPU path
	int totalThreads = 2;

	//Parse command-line arguments
	string flag, value;

	for(int i = 0; i < argc; i++)
	{
		flag = argv[i];

		if((i + 1) < argc)
			value = argv[i + 1];

		//Set the password string to be cracked
		if(flag == "-h" || flag == "--help")
		{
			printHelp();
			return 0;
		}

		//Take an NTLM hash and crack it
		if(flag == "-b")
		{
			string newHash = value;

			newHash = toLower(newHash);
 
			//Check to see whether the hash has been entered correctly
			//The length of a proper NTLM hash is always 32 characters
			if(newHash.length() == 32)
			{
				hashTemp = newHash;
			}
			else
			{
				cerr << "\nERROR: Invalid hash! String must be 32 characters in length." << endl;
			}
		}

		//Take a plain text string, generate an NTLM hash of it, and crack it
		if(flag == "-s")
		{
			NTLM ntlm;
			hashTemp = ntlm.getNTLMHash(value);
		}

		//Set the number of threads
		if(flag == "-t")
		{
			string temp;
			temp = value;

			if(toInt(temp) >= 2)
			{
				totalThreads = toInt(temp);
			}
			else
			{
				totalThreads = 2;
			}
		}

		if(flag == "-c")
		{
			processingPath::setMaxChars(toInt(value));
		}

		//Interval for iteration logging
		if(flag == "-i")
		{
			interval = toInt(value);
			masterThread::setInterval(interval);
		}
		else
		{
			masterThread::setInterval(interval);
		}

		//Disable iteration logging
		if(flag == "--silent")
		{
			masterThread::setSilent(true);
		}
		else 
		{
			silent = false;
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
			masterThread::setDisableLookup(true);
		}

		//Disable threading
		if(flag == "--disable-threading")
		{
			totalThreads = 1;
		}
	}

	//Check to see that we have a valid target
	if(hashTemp.length() > 0)
	{
		cout << "\nRunning " << totalThreads << " (+1) cooperative threads," << endl
			 << "Cracking NTLM hash " << hashTemp << ".\n\n";

		processingPath::setTarget(hashTemp);
	}
	else
	{
		cerr << "\nERROR: Invalid input. Please choose a valid action." << endl
			 << "Use flag '-h' to see the possible options." << endl;

		return 1;
	}

	//The thread group is like a big pile of zombie slaves which rise to do your bidding with a simple "join_all()"
	//You can add a thread--or a thousand--per processing path. The latter scenario is not suggested for paths which
	//are threaded by nature. (GPUs, for example. Try sending a GPU instructions from multiple threads, and you'll crash the app.
	boost::thread_group threadGroup;

	//Set the total number of worker threads in use for this run
	processingPath::setTotalThreads(totalThreads);

	//Add a master thread to the group
	threadGroup.create_thread(masterThread());

	//Create a number of threads for the CPU path
	for(int i = 0; i < totalThreads; i++)
	{
		threadGroup.create_thread(CPUPath(i));
	}

	threadGroup.join_all();

	return 0;
}