#include <iostream>
#include <vector>

#include "NTLM_MD.h"

using namespace std;

int main()
{
    NTLM_SSE2 ntlmmd;
    NTLM ntlm;
    
    std::string test = "testString";
    
    std::string input[12];
    int64_pair output[12];
    
    for(int i = 0; i < 12; i++)
        input[i] = test;
    
    ntlmmd.getMultipleWeakHashes(input, output);
    
    int64_pair vanillaResult = ntlm.getWeakHash(test);
    
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
        cout << "SSE2 accelerated NTLM algorithm passed the test." << endl;
    }
    else
    {
        cout << "SSE2 accelerated NTLM algorithm FAILED the test." << endl;
        
        for(int i = 0; i < failedHashes.size(); i++)
            cout << "\nString " << failedHashes[i] << " was not hashed properly." << endl;
    }
    
    return 0;
}