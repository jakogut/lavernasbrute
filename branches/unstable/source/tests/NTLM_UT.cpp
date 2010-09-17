#include <iostream>
#include <vector>

#include "MD4.h"

using namespace std;

int main()
{
	MD4 md4;

	const char* input = "testString";

	const char* correctFullHash = "7c404e7a355530e496d6f5612a1ed54e";
	int64_pair correctWeakHash = md4.weakenHash(correctFullHash);

	// Test for vanilla C full NTLM algorithm
	if(md4.getHash_NTLM(input) == correctFullHash)
	{
		cout << "Vanilla NTLM algorithm passed full hashing test." << endl;
	}
	else
	{
		cout << "Vanilla NTLM algorithm FAILED full hashing test." << endl;
	}

	// Test for vanilla C partial NTLM algorithm
	if(md4.getWeakHash_NTLM(input) == correctWeakHash)
	{
		cout << "Vanilla NTLM algorithm passed partial hashing test." << endl;
	}
	else
	{
		cout << "Vanilla NTLM algorithm FAILED partial hashing test." << endl;
	}
}

