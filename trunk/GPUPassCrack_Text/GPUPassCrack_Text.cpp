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

//Number of threads - At least two are required for proper operation.
//One thread is a master thread which monitors the workers, all the rest are workers.
#define NTHREADS 8

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
string threads::passwd = "";

int threads::max = 0;

//Create mutexes to control those nasty filthy threads trying to hog control of resources all for
//themselves. They should be ashamed.
boost::recursive_mutex threads::readIterationsMutex;
boost::recursive_mutex threads::writeIterationsMutex;
boost::recursive_mutex threads::readPasswdMutex;
boost::recursive_mutex threads::writePasswdMutex;
boost::recursive_mutex threads::readPasswdNumericalStringMutex;
boost::recursive_mutex threads::writePasswdNumericalStringMutex;

boost::recursive_mutex masterThread::readSuccessMutex;
boost::recursive_mutex masterThread::writeSuccessMutex;

//If the counter goes over the limits of a normal integer, the program crashes.
//This really sucks when it's been running for hours. I'm using a 64-bit integer now.
__int64 threads::iterations = 0;

string threads::passwdNumericalString = "";

//////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	string passwdTemp = "passwd";

	//cout << "Input a string to brute-force: ";
	//cin >> passwdTemp;

	cout << "Brute forcing string \"" << passwdTemp << "\"\nRunning " << NTHREADS << " (+1) cooperative threads." << endl;
	threads::writePasswd(passwdTemp);

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