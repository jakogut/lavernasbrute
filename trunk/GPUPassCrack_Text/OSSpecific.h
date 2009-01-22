#ifndef OSCRACK_H_
#define OSCRACK_H_

#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

class OSSpecific
{
public:

	OSSpecific();
	~OSSpecific();

	class windows
	{
	public:

		windows();
		~windows();

		static void dumpNTLMHashes(string filename);
		static void importNTLMHashes(string filename);
	};

	class linux;
};

#endif