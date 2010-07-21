//Part of Laverna's Brute

#ifndef MASTERTHREAD_H_
#define MASTERTHREAD_H_

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <iomanip>

#include <boost/thread/thread.hpp>

#include "CharacterSet.h"
#include "ProcessingPath.h"
#include "Version.h"

#include <Pow.h>

class masterThread
{
public:

	masterThread();
	~masterThread();

	void operator()();

	static bool getSuccess();
	static void setSuccess();

	static bool getSilent();
	static void setSilent(bool input);

	static int getNumWorkers();

	static void setRemainingTargets(int input);

	static unsigned long long getIterations();

	static void initCharset(int min, int max, 
							int charsec0, int charsec1, int charsec2, int charsec3);

	static characterSet* getCharset();

	static void printResult(std::string hash, std::string plaintext);

	void printStatistics();

	// Thread management
	static processingPath* getWorkerPtr(int id);
	static void manageWorker(processingPath* worker);

	// The return value is whether or not the master was able to create new work for the idle thread
	static bool reassignKeyspace(processingPath* worker);

	static unsigned long long getRemainingKeyspace(int id);
	static unsigned long long getRemainingKeyspace(processingPath* worker);

protected:

	int id;
	time_t startTime;

	static std::map<int, processingPath*> workerPtrMap;

	static bool silent;

	//When a thread is successful, it sets this value to true, signaling the other threads
	//That the job is done.
	static bool success;

	static characterSet charset;

	static int remainingTargets;

	static std::vector< std::pair<std::string, std::string> > results;

	static boost::mutex stdoutMutex;
};

#endif
