#include <iostream>
#include <vector>

#include "MD4_SSE.h"

using namespace std;

int main()
{
	MD4_SSE md4_md;
	MD4 md4;

	char* test = "testString";

	char* input[12];
	int64_pair output[12];

	for(int i = 0; i < 12; i++)
	{
		input[i] = new char[16];
		strcpy(input[i], test);
	}

	md4_md.getWeakHashes_NTLM(input, output);

	int64_pair vanillaResult = md4.getWeakHash_NTLM(test);

	bool passed = true;
	vector<int> failedHashes;

	for(int i = 0; i < 12; i++)
	{
		if(output[i] != vanillaResult)
		{
			passed = false;
			failedHashes.push_back(i);
		}
	}

	if(passed)
	{
	cout << "SSE accelerated NTLM algorithm passed the test." << endl;
	}
	else
	{
		cout << "SSE accelerated NTLM algorithm FAILED the test." << endl;

		for(unsigned int i = 0; i < failedHashes.size(); i++)
			cout << "\nString " << failedHashes[i] << " was not hashed properly." << endl;
	}

	for(int i = 0; i < 12; i++) delete [] input[i];

	return 0;
}

