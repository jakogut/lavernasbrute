#ifndef THREADS_H_
#define THREADS_H_

//Absolute minimum
#define MIN 110

//Lowercase character set only
#define MAX 389

//Lowercase & uppercase character sets
//#define MAX 679

//Lowercase, uppercase, & numbers
//#define MAX 789

#include <iostream>
#include <stdlib.h>
#include <time.h>
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

	threads::threads(int id) : id(id){};

	void operator()();
	
	static __int64 getIterations();
	int roundToNearestTen(int round);
	int id;
	static int max;

	static void incrementIterations();
	static void writePasswd(string write);
	static void setPasswdNumericalString(string write);

	void generatePasswdString(string passwd);
	
	static string getPasswdNumericalString();
	static string getPasswd();

	string replaceAll(string source, string a, string b);
	string generateRandString(int length);

	bool findCharSequence(string a, string b);
	string parseNumericalString(string passwdNumericalString);

	static __int64 iterations;
	static string passwdNumericalString;

	static boost::recursive_mutex readIterationsMutex;
	static boost::recursive_mutex writeIterationsMutex;
	static boost::recursive_mutex readPasswdMutex;
	static boost::recursive_mutex writePasswdMutex;
	static boost::recursive_mutex readPasswdNumericalStringMutex;
	static boost::recursive_mutex writePasswdNumericalStringMutex;

	static string passwd;
};

#endif
