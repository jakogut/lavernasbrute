#ifndef THREADS_H_
#define THREADS_H_

#define MIN 48		
#define MAX 122		

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string>

#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/random.hpp>

extern int random();

using namespace std;

class threads
{
public:

	threads(int id) 
	 : id(id)
	{
	}

	void operator()();
	
	int id;
	static int max;
	
	//Our most oft called functions, we're inlining them for a small, but not negligible, speed improvement.
	inline static void incrementIterations();
	inline static string getPasswdNumericalString();
	inline static void setPasswdNumericalString(string write);

	static __int64 getIterations();
	static void writePasswd(string write);
	
	void generatePasswdString(string passwd);
	
	static string getPasswd();

	inline string generateRandString(int length);

	static __int64 iterations;
	static string passwdNumericalString;

	static boost::mutex IterationsMutex;
	static boost::recursive_mutex PasswdMutex;
	static boost::recursive_mutex PasswdNumericalStringMutex;
	static boost::recursive_mutex generatePasswdStringMutex;

	static string passwd;
};

#endif
