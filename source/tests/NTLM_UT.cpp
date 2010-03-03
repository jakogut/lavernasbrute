#include <iostream>
#include <vector>

#include "NTLM.h"

using namespace std;

int main()
{
    NTLM ntlm;
    
    std::string input = "testString";
    
    std::string correctFullHash = "7c404e7a355530e496d6f5612a1ed54e";
    int64_pair correctWeakHash = ntlm.weakenHash(correctFullHash);
    
    // Test for vanilla C full NTLM algorithm
    if(ntlm.getNTLMHash(input) == correctFullHash)
    {
        cout << "Vanilla NTLM algorithm passed full hashing test." << endl;
    }
    else
    {
        cout << "Vanilla NTLM algorithm FAILED full hashing test." << endl;
    }
    
    // Test for vanilla C partial NTLM algorithm
    if(ntlm.getWeakHash(input) == correctWeakHash)
    {
        cout << "Vanilla NTLM algorithm passed partial hashing test." << endl;
    }
    else
    {
        cout << "Vanilla NTLM algorithm FAILED partial hashing test." << endl;
    }
}