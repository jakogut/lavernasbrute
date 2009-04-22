//Part of Laverna's Brute
//My code isn't commented all that well in some parts; my apologies. I understand all of it, and that's what matters. :)
//I'll work on commenting it better after I've written some more code...

#include <iostream>
#include <time.h>
#include <string>

//Boost thread
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>

//Got to have a slave driver
#include "MasterThread.h"

//Processing path base class
#include "ProcessingPath.h"

//Processing paths
#include "CPUPath.h"
#include "GPUPath.h"

using namespace std;

////////////////////////////////
// VARIABLES ///////////////////
////////////////////////////////

//Initialize our static variables:
//////////////////////////////////////////////////////////////////////////////////////////

bool masterThread::success = false;
bool masterThread::silent = false;
int masterThread::interval;
string masterThread::crackedPassword;
char* masterThread::randCharset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
int masterThread::charsetLength = strlen(randCharset);

string processingPath::target = "";
int processingPath::max = 0;
bool processingPath::randFast = false;

//Create mutexes to control those nasty filthy threads trying to hog control of resources all for
//themselves. They should be ashamed.

boost::try_mutex masterThread::SuccessMutex;
boost::mutex masterThread::printMutex;

//A counter to log the number of iterations run
long long masterThread::iterations = 0;

//////////////////////////////////////////////////////////////////////////////////////////

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
	"\n\n-t INTEGER\tNumber of worker threads used in parallel calculation."
	"\n\n-i INTEGER\tInterval for iterations logged to the console; measured in seconds."
	"\n\t\tThe interval may be raised for a slight performance gain."
	"\n\n--silent\tRun the program in silent mode."
	"\n\n--linear\tRun the program using a faster, but more linear RNG."
	"\n\t\tNOT THREAD SAFE - No more than two threads will be used."
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

int main(int argc, char* argv[])
{
	bool silent;
	string hashTemp = "";
	int interval = 5;

	//Number of threads for the CPU path
	int NTHREADS = 128;

	//Parse command-line arguments
	for(int i = 0; i < argc; ++i)
	{
		//Set the password string to be cracked
		if(strcmp(argv[i], "-h") == 0)
		{
			printHelp();
			return 0;
		}

		//Disable iteration logging
		if(strcmp(argv[i], "--silent") == 0)
		{
			silent = true;
			masterThread::setSilent(silent);
		}
		else 
		{
			silent = false;
		}

		//Take an NTLM hash and crack it
		if(strcmp(argv[i], "-b") == 0)
		{
			string newHash = argv[i + 1];
 
			//Check to see whether the hash has been entered correctly
			//The length of a proper NTLM hash is always 32 characters
			if(newHash.length() == 32)
			{
				hashTemp = argv[i + 1];
			}
			else
			{
				cerr << "\nERROR: Invalid hash! String must be 32 characters in length." << endl;
			}
		}

		//Take a plain text string, generate an NTLM hash of it, and crack it
		if(strcmp(argv[i], "-s") == 0)
		{
			NTLM* mNTLM = new NTLM();
			hashTemp = mNTLM->getNTLMHash(argv[i + 1]);

			delete mNTLM;
		}

		//Set the number of threads
		if(strcmp(argv[i], "-t") == 0)
		{
			string temp;
			temp = argv[i + 1];

			if(toInt(temp) >= 2)
			{
				NTHREADS = toInt(temp);
			}
			else
			{
				NTHREADS = 2;
			}
		}

		//Interval for iteration logging
		if(strcmp(argv[i], "-i") == 0)
		{
			string temp;
			temp = argv[i + 1];

			interval = toInt(temp);
			masterThread::setInterval(interval);
		}
		else
		{
			masterThread::setInterval(interval);
		}

		//Disable threading
		if(strcmp(argv[i], "--disable-threading") == 0)
		{
			NTHREADS = 1;
		}

		//Use the standard library RNG
		if(strcmp(argv[i], "--linear") == 0)
		{
			processingPath::useStandardRand(true);

			//Seed our RNG
			srand((unsigned)time(NULL));

			//This RNG is not thread-safe, ensure that we're using two or less threads.
			if(NTHREADS > 2)
			{
				NTHREADS = 2;
			}
		}
		else
		{
			processingPath::useStandardRand(false);
		}
	}

	if(hashTemp.length() <= 0)
	{
		printHelp();
		return 0;
	}
	else
	{
		cout << "\nRunning " << NTHREADS << " (+1) cooperative threads." << endl
			<< "Cracking NTLM hash " << hashTemp << ".\n\n";

		processingPath::setTarget(hashTemp);
	}

	//Start the clock
	time_t startTime = time(NULL);

	//The thread group is like a big pile of zombie slaves which rise to do your bidding with a simple "join_all()"
	//You can add a thread--or a thousand--per processing path. The latter scenario is not suggested for paths which
	//are threaded by nature. (GPUs, for example. Try sending a GPU instructions from multiple threads, and you'll crash the app.
	boost::thread_group threadGroup;

	//Add a master thread to the group
	threadGroup.create_thread(masterThread(0, startTime));

	//Add a thread for our Stream processing path
	//threadGroup.create_thread(GPUPath(1));		

	//Create a number of threads for the CPU path
	for(int i = 0; i < NTHREADS; ++i)
	{
		threadGroup.create_thread(CPUPath(i + 2));
	}

	threadGroup.join_all();

	return 0;
}