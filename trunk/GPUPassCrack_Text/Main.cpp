#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/random.hpp>

#include "MasterThread.h"
#include "Threads.h"

using namespace std;

////////////////////////////////
// VARIABLES ///////////////////
////////////////////////////////

boost::recursive_mutex mutex;

//Setup Random(ish) Number Generator
typedef boost::uniform_int<> UniformDist;
UniformDist dist(MIN,MAX); 

typedef boost::mt19937 BaseRNG;
BaseRNG rng; 

boost::variate_generator<BaseRNG&,UniformDist>
getRand(rng, dist); 

int random()
{
	return getRand();
}

//Initialize our static variables:
//////////////////////////////////////////////////////////////////////////////////////////

bool masterThread::success = false;
bool masterThread::silent = false;
int masterThread::interval;

string threads::passwd = "";
int threads::max = 0;

//Create mutexes to control those nasty filthy threads trying to hog control of resources all for
//themselves. They should be ashamed.
boost::mutex threads::IterationsMutex;
boost::recursive_mutex threads::PasswdMutex;
boost::recursive_mutex threads::PasswdNumericalStringMutex;
boost::recursive_mutex threads::generatePasswdStringMutex;

boost::recursive_mutex masterThread::SuccessMutex;
boost::timed_mutex masterThread::printMutex;

//If the counter goes over the limits of a normal integer, the program crashes.
//This really sucks when it's been running for hours. I'm using a 64-bit integer now.
__int64 threads::iterations = 0;

string threads::passwdNumericalString = "";

//////////////////////////////////////////////////////////////////////////////////////////

void printHelp()
{
	cout << "\nLaverna's Brute."
	"\nThis program is a brute-force password cracker."
	"\nCOPYRIGHT 2008, this program is licensed under the GNU GPL V3 "
	"\nSee \"http://lavernasbrute.googlecode.com\" for more details. "
	"\n\nI am NOT responsible for loss of or damage to personal property as a result of "
	"\nthe use of this program. Any use of this program is at your own risk."
	"\n\n-h\t\tDisplay this help message. "
	"\n\n-s STRING\tPass a text string to the brute-forcer for testing purposes."
	"\n\n-t INTEGER\tNumber of threads, Two are required minimum. Default: 8"
	"\n\n-i INTEGER\tInterval in iterations logged to the console. Default: 1,000,000"
	"\n\t\tThe interval may be raised for a slight performance gain.\n\n"
	"\n\n--silent\t\tRun the program in silent mode."
	"\n\n--disable-threading\t Disables threading.\n\n";
}

int main(int argc, char* argv[])
{
	bool silent;
	string passwdTemp = "";
	int interval = 1000000;

	//Number of threads - At least two are required for proper operation.
	//One thread is a master thread which monitors the workers, all the rest are workers.
	int NTHREADS = 8;

	//Parse command-line arguments
	for(int i = 0; i < argc; i++)
	{
		//Set the password string to be cracked
		if(strcmp(argv[i], "-h") == 0)
		{
			printHelp();
			return 0;
		}

		//Set silent mode
		if(strcmp(argv[i], "--silent") == 0)
		{
			silent = true;
			masterThread::setSilent(silent);
		}
		else 
		{
			silent = false;
		}

		//Set the password string to be cracked
		if(strcmp(argv[i], "-s") == 0)
		{
			passwdTemp = argv[i + 1];
		}

		//Set the number of threads
		if(strcmp(argv[i], "-t") == 0)
		{
			char* temp = new char[]; 
			temp = argv[i + 1];

			if(atoi(temp) >= 2)
			{
				NTHREADS = atoi(temp);
			}
			else
			{
				NTHREADS = 2;
			}

			delete[] temp;
		}

		//Interval
		if(strcmp(argv[i], "-i") == 0)
		{
			char* temp = new char[]; 
			temp = argv[i + 1];

			interval = atoi(temp);
			masterThread::setInterval(interval);

			delete[] temp;
		}
		else
		{
			masterThread::setInterval(interval);
		}

		//Disable threading (Use one thread)
		if(strcmp(argv[i], "--disable-threading") == 0)
		{
			NTHREADS = 1;
		}
	}

	if(passwdTemp.length() <= 0)
	{
		printHelp();
		return 0;
	}
	else
	{
		cout << "\nBrute forcing string \"" << passwdTemp << "\"\nRunning " << NTHREADS << " (+1) cooperative threads." << endl;
		threads::writePasswd(passwdTemp);
	}

	cout << "\n\n";

	//Seed our RNG
	rng.seed((unsigned)time(NULL));

	//Start the clock
	time_t startTime = time(NULL);

	boost::thread_group threadGroup;
	threadGroup.create_thread(masterThread(0, startTime));

	for(int i = 0; i < NTHREADS; i++)
	{
		threadGroup.create_thread(threads(i));
	}

	threadGroup.join_all();

	return 0;
}