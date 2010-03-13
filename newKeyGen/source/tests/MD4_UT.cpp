#include <iostream>
#include <vector>

#include "MD4.h"

using namespace std;

int main()
{
    MD4 md4;

    std::string input = "testString";
    
    std::string correctFullHash = "d25cd189889817578d36756cd6801bc5";
    int64_pair correctWeakHash = md4.weakenHash(correctFullHash);
    
    // Test for vanilla C full MD4 algorithm
    if(md4.getMD4Hash(input) == correctFullHash)
    {
        cout << "Vanilla MD4 algorithm passed full hashing test." << endl;
    }
    else
    {
        cout << "Vanilla MD4 algorithm FAILED full hashing test." << endl;
    }
    
    // Test for vanilla C partial MD4 algorithm
    if(md4.getWeakMD4Hash(input) == correctWeakHash)
    {
        cout << "Vanilla MD4 algorithm passed partial hashing test." << endl;
    }
    else
    {
        cout << "Vanilla MD4 algorithm FAILED partial hashing test." << endl;
    }
}

