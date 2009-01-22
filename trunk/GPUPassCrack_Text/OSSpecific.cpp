#include "OSSpecific.h"

using namespace std;

//Dirty hacky, but it works. This dumps the NTLM hashes using pwdump.
void OSSpecific::windows::dumpNTLMHashes(string filename)
{
	string command = "PwDump -o ";
	command.append(filename.c_str());
	command.append(" 127.0.0.1");

	cout << "Dumping password hashes..." << endl;
	system(command.c_str());
	cout << endl;
}

void OSSpecific::windows::importNTLMHashes(string filename)
{
}