#include <iostream>
#include "hashing/MD4_CUDA.cuh"
#include "hashing/MD4.h"

#include "messageGenerators/MessageGenerator_NTLM.h"

#define NUM_HASHES 512

using namespace std;

int main()
{
	characterSet* charset = createCharacterSet('a', 'z', 0, 0, 0, 0);
	messageGenerator_NTLM messgen(charset);

	hashContext correct_ctx[NUM_HASHES];
	hashContext trial_ctx[NUM_HASHES];

	for(int i = 0; i < NUM_HASHES; i++)
	{
		messgen.integerToMessage(&correct_ctx[i], i);
		messgen.integerToMessage(&trial_ctx[i], i);
	}

	MD4 md4;
	MD4_CUDA md4_cuda;

	for(int i = 0; i < NUM_HASHES; i++) md4.getHashContext(&correct_ctx[i]);

	md4_cuda.getHashContext(trial_ctx, NUM_HASHES);

	int i;
	bool match = true;
	for(i = 0; i < NUM_HASHES; i++)
	{
		if(trial_ctx[i].wd[0] != correct_ctx[i].wd[0])
		{
			match = false;
			break;
		}
	}

	cout << match << endl;

	if(!match) cout << i << endl;
	for(int j = 0; j < 4; j++) cout << correct_ctx[i].wd[j] << "\t" << trial_ctx[i].wd[j] << endl;

	destroyCharacterSet(charset);
}
