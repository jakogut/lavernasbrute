#ifndef MD4_SSE_H_
#define MD4_SSE_H_

#include <emmintrin.h>

#include "MD4.h"
#include "KeyGenerator.h"

#define round1_SSE(set, wd_index, a, b, c, ntb_index, rotation) \
	wd_SSE[set][wd_index] = _mm_add_epi32(wd_SSE[set][wd_index], _mm_add_epi32(_mm_or_si128(_mm_and_si128(wd_SSE[set][a], wd_SSE[set][b]), _mm_andnot_si128(wd_SSE[set][a], wd_SSE[set][c])), message_SSE[set][ntb_index])), \
	wd_SSE[set][wd_index] = ROTL_SSE(wd_SSE[set][wd_index], rotation)

#define round2_SSE(set, wd_index, a, b, c, ntb_index, rotation) \
	wd_SSE[set][wd_index] = _mm_add_epi32(wd_SSE[set][wd_index], _mm_add_epi32(_mm_add_epi32(_mm_or_si128(_mm_and_si128(wd_SSE[set][a], _mm_or_si128(wd_SSE[set][b], wd_SSE[set][c])), _mm_and_si128(wd_SSE[set][b], wd_SSE[set][c])), message_SSE[set][ntb_index]), SQRT_2_sse)), \
	wd_SSE[set][wd_index] = ROTL_SSE(wd_SSE[set][wd_index], rotation)

#define round3_SSE(set, wd_index, a, b, c, ntb_index, rotation) \
	wd_SSE[set][wd_index] = _mm_add_epi32(_mm_add_epi32(_mm_add_epi32(wd_SSE[set][wd_index], _mm_xor_si128(wd_SSE[set][a], _mm_xor_si128(wd_SSE[set][b], wd_SSE[set][c]))), message_SSE[set][ntb_index]), SQRT_3_sse), \
	wd_SSE[set][wd_index] = ROTL_SSE(wd_SSE[set][wd_index], rotation)

/* The rounds with "null" in the name omit the adding of the message section, 
because if the hash is short enough to be computationally feasible to crack, 
that element of the message will be zero. Since each ascii character is 8 bits,
and each unicode character is 16 bits, this means that each 32-bit word that composes 
the message can store either four ascii characters per word, or two unicode characters 
per word. If it is computationally infeasible to attack the hash to any plaintext more 
than 14 characters in length, that means we can assume that every element beyond the 4th
for MD4, and 7th for NTLM are zeros.*/

#define round1_null_SSE(set, wd_index, a, b, c, ntb_index, rotation) \
	wd_SSE[set][wd_index] = _mm_add_epi32(wd_SSE[set][wd_index], _mm_or_si128(_mm_and_si128(wd_SSE[set][a], wd_SSE[set][b]), _mm_andnot_si128(wd_SSE[set][a], wd_SSE[set][c]))), \
	wd_SSE[set][wd_index] = ROTL_SSE(wd_SSE[set][wd_index], rotation)

#define round2_null_SSE(set, wd_index, a, b, c, ntb_index, rotation) \
	wd_SSE[set][wd_index] = _mm_add_epi32(wd_SSE[set][wd_index], _mm_add_epi32(_mm_or_si128(_mm_and_si128(wd_SSE[set][a], _mm_or_si128(wd_SSE[set][b], wd_SSE[set][c])), _mm_and_si128(wd_SSE[set][b], wd_SSE[set][c])), SQRT_2_sse)), \
	wd_SSE[set][wd_index] = ROTL_SSE(wd_SSE[set][wd_index], rotation)

#define round3_null_SSE(set, wd_index, a, b, c, ntb_index, rotation) \
	wd_SSE[set][wd_index] = _mm_add_epi32(_mm_add_epi32(wd_SSE[set][wd_index], _mm_xor_si128(wd_SSE[set][a], _mm_xor_si128(wd_SSE[set][b], wd_SSE[set][c]))), SQRT_3_sse), \
	wd_SSE[set][wd_index] = ROTL_SSE(wd_SSE[set][wd_index], rotation)

#define ROTL_SSE(num, places) (_mm_or_si128(_mm_slli_epi32(num, places), _mm_srli_epi32(num, (32 - places))))

class MD4_SSE : public MD4
{
public:

	MD4_SSE()
	{
		SQRT_2_sse = _mm_set1_epi32(0x5a827999);
		SQRT_3_sse = _mm_set1_epi32(0x6ed9eba1);
	}

	~MD4_SSE()
	{
	}

	typedef char* (MD4::*vectorHashPtr)(const char*);

	// Not quite done yet
	/*void gethashContext(char** input, hashContext* output)
	{
		prepareKey_MD4_SSE(input);

		initialize();
		encrypt();

		convertToContext_SSE(output);
	}*/

	void getHashContext_NTLM(char** input, hashContext* output)
	{
		prepareKey_NTLM_SSE(input);

		initialize();
		encrypt();

		convertToContext_SSE(output);
	}

protected:

	/*inline void prepareKey_MD4_SSE(char** input)
	{
		unsigned int length[3][4];

		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 4; j++)
				length[i][j] = (unsigned int)input[j+4*i].length();

		for(int i = 0; i < 3; i++)
		{
			memset(message,0,16*4*4);

			for(int j = 0; j < 4; j++)
			{
				unsigned int k=0;
				//The length of input need to be <= 27
				for(;k<length[i][j]/4;k++)	
					message[k][j] = input[j+4*i][4*k] | (input[j+4*i][4*k+1]<<8) | (input[j+4*i][4*k+2]<<16) | (input[j+4*i][4*k+3]<<24);
			 
				//padding
				switch(length[i][j]%4)
				{
				case 0:
					message[k][j] = 0x80;
					break;
				case 1:
					message[k][j] = input[j+4*i][length[i][j]-1] | 0x8000;
					break;
				case 2:
					message[k][j] = input[j+4*i][length[i][j]-2] | (input[j+4*i][length[i][j]-1]<<8) | 0x800000;
					break;
				case 3:
					message[k][j] = input[j+4*i][length[i][j]-3] | (input[j+4*i][length[i][j]-2]<<8) | (input[j+4*i][length[i][j]-1]<<16) | 0x80000000;
					break;
				}

				//put the length
				message[14][j] = length[i][j] << 3;
			}

			for(int j = 0; j < 3; j++)
				message_SSE[i][j] = _mm_load_si128((__m128i*)message[j]);
		}
	}*/

	inline void prepareKey_NTLM_SSE(char** input)
	{
		unsigned int length[3][4];

		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 4; j++)
				length[i][j] = (unsigned int)strlen(input[j+4*i]);

		for(int i = 0; i < 3; i++)
		{
			memset(message, 0, 16*4*4);

			for(int j = 0; j < 4; j++)
			{
				unsigned int k = 0;
				//The length of input need to be <= 27
				for(;k<length[i][j]/2;k++)	
					message[k][j] = input[j+4*i][2*k] | (input[j+4*i][2*k+1]<<16);
			 
				//padding
				if(length[i][j]%2==1)
					message[k][j] = input[j+4*i][length[i][j]-1] | 0x800000;
				else
					message[k][j]=0x80;
				//put the length
				message[14][j] = length[i][j] << 4;
			}

			for(int j = 0; j < 16; j++)
				message_SSE[i][j] = _mm_load_si128((__m128i*)message[j]);
		}
	}

	inline void initialize()
	{
		for(int i = 0; i < 3; i++)
		{
			wd_SSE[i][0] = _mm_set1_epi32(wd_init[0]);
			wd_SSE[i][1] = _mm_set1_epi32(wd_init[1]);
			wd_SSE[i][2] = _mm_set1_epi32(wd_init[2]);
			wd_SSE[i][3] = _mm_set1_epi32(wd_init[3]);
		}
	}

	inline void encrypt()
	{	 
		// Round 1 // ---

		round1_SSE(0, 0, 1, 2, 3, 0, 3);
		round1_SSE(1, 0, 1, 2, 3, 0, 3);
		round1_SSE(2, 0, 1, 2, 3, 0, 3);
		round1_SSE(0, 3, 0, 1, 2, 1, 7);
		round1_SSE(1, 3, 0, 1, 2, 1, 7);
		round1_SSE(2, 3, 0, 1, 2, 1, 7);
		round1_SSE(0, 2, 3, 0, 1, 2, 11);
		round1_SSE(1, 2, 3, 0, 1, 2, 11);
		round1_SSE(2, 2, 3, 0, 1, 2, 11);
		round1_SSE(0, 1, 2, 3, 0, 3, 19);
		round1_SSE(1, 1, 2, 3, 0, 3, 19);
		round1_SSE(2, 1, 2, 3, 0, 3, 19);
		

		round1_SSE(0, 0, 1, 2, 3, 4, 3);
		round1_SSE(1, 0, 1, 2, 3, 4, 3);
		round1_SSE(2, 0, 1, 2, 3, 4, 3);
		round1_SSE(0, 3, 0, 1, 2, 5, 7);
		round1_SSE(1, 3, 0, 1, 2, 5, 7);
		round1_SSE(2, 3, 0, 1, 2, 5, 7);
		round1_SSE(0, 2, 3, 0, 1, 6, 11);
		round1_SSE(1, 2, 3, 0, 1, 6, 11);
		round1_SSE(2, 2, 3, 0, 1, 6, 11);
		round1_SSE(0, 1, 2, 3, 0, 7, 19);
		round1_SSE(1, 1, 2, 3, 0, 7, 19);
		round1_SSE(2, 1, 2, 3, 0, 7, 19);
		

		round1_null_SSE(0, 0, 1, 2, 3, 8, 3);
		round1_null_SSE(1, 0, 1, 2, 3, 8, 3);
		round1_null_SSE(2, 0, 1, 2, 3, 8, 3);
		round1_null_SSE(0, 3, 0, 1, 2, 9, 7);
		round1_null_SSE(1, 3, 0, 1, 2, 9, 7);
		round1_null_SSE(2, 3, 0, 1, 2, 9, 7);
		round1_null_SSE(0, 2, 3, 0, 1, 10, 11);
		round1_null_SSE(1, 2, 3, 0, 1, 10, 11);
		round1_null_SSE(2, 2, 3, 0, 1, 10, 11);
		round1_null_SSE(0, 1, 2, 3, 0, 11, 19);
		round1_null_SSE(1, 1, 2, 3, 0, 11, 19);
		round1_null_SSE(2, 1, 2, 3, 0, 11, 19);
		

		round1_null_SSE(0, 0, 1, 2, 3, 12, 3);
		round1_null_SSE(1, 0, 1, 2, 3, 12, 3);
		round1_null_SSE(2, 0, 1, 2, 3, 12, 3);
		round1_null_SSE(0, 3, 0, 1, 2, 13, 7);
		round1_null_SSE(1, 3, 0, 1, 2, 13, 7);
		round1_null_SSE(2, 3, 0, 1, 2, 13, 7);
		round1_SSE(0, 2, 3, 0, 1, 14, 11);
		round1_SSE(1, 2, 3, 0, 1, 14, 11);
		round1_SSE(2, 2, 3, 0, 1, 14, 11);
		round1_null_SSE(0, 1, 2, 3, 0, 15, 19);
		round1_null_SSE(1, 1, 2, 3, 0, 15, 19);
		round1_null_SSE(2, 1, 2, 3, 0, 15, 19);
		

		// Round 2 // ---

		round2_SSE(0, 0, 1, 2, 3, 0, 3);
		round2_SSE(1, 0, 1, 2, 3, 0, 3);
		round2_SSE(2, 0, 1, 2, 3, 0, 3);
		round2_SSE(0, 3, 0, 1, 2, 4, 5);
		round2_SSE(1, 3, 0, 1, 2, 4, 5);
		round2_SSE(2, 3, 0, 1, 2, 4, 5);
		round2_null_SSE(0, 2, 3, 0, 1, 8, 9);
		round2_null_SSE(1, 2, 3, 0, 1, 8, 9);
		round2_null_SSE(2, 2, 3, 0, 1, 8, 9);
		round2_null_SSE(0, 1, 2, 3, 0, 12, 13);
		round2_null_SSE(1, 1, 2, 3, 0, 12, 13);
		round2_null_SSE(2, 1, 2, 3, 0, 12, 13);
		

		round2_SSE(0, 0, 1, 2, 3, 1, 3);
		round2_SSE(1, 0, 1, 2, 3, 1, 3);
		round2_SSE(2, 0, 1, 2, 3, 1, 3);
		round2_SSE(0, 3, 0, 1, 2, 5, 5);
		round2_SSE(1, 3, 0, 1, 2, 5, 5);
		round2_SSE(2, 3, 0, 1, 2, 5, 5);
		round2_null_SSE(0, 2, 3, 0, 1, 9, 9);
		round2_null_SSE(1, 2, 3, 0, 1, 9, 9);
		round2_null_SSE(2, 2, 3, 0, 1, 9, 9);
		round2_null_SSE(0, 1, 2, 3, 0, 13, 13);
		round2_null_SSE(1, 1, 2, 3, 0, 13, 13);
		round2_null_SSE(2, 1, 2, 3, 0, 13, 13);
		

		round2_SSE(0, 0, 1, 2, 3, 2, 3);
		round2_SSE(1, 0, 1, 2, 3, 2, 3);
		round2_SSE(2, 0, 1, 2, 3, 2, 3);
		round2_SSE(0, 3, 0, 1, 2, 6, 5);
		round2_SSE(1, 3, 0, 1, 2, 6, 5);
		round2_SSE(2, 3, 0, 1, 2, 6, 5);
		round2_null_SSE(0, 2, 3, 0, 1, 10, 9);
		round2_null_SSE(1, 2, 3, 0, 1, 10, 9);
		round2_null_SSE(2, 2, 3, 0, 1, 10, 9);
		round2_SSE(0, 1, 2, 3, 0, 14, 13);
		round2_SSE(1, 1, 2, 3, 0, 14, 13);
		round2_SSE(2, 1, 2, 3, 0, 14, 13);
		

		round2_SSE(0, 0, 1, 2, 3, 3, 3);
		round2_SSE(1, 0, 1, 2, 3, 3, 3);
		round2_SSE(2, 0, 1, 2, 3, 3, 3);
		round2_SSE(0, 3, 0, 1, 2, 7, 5);
		round2_SSE(1, 3, 0, 1, 2, 7, 5);
		round2_SSE(2, 3, 0, 1, 2, 7, 5);
		round2_null_SSE(0, 2, 3, 0, 1, 11, 9);
		round2_null_SSE(1, 2, 3, 0, 1, 11, 9);
		round2_null_SSE(2, 2, 3, 0, 1, 11, 9);
		round2_null_SSE(0, 1, 2, 3, 0, 15, 13);
		round2_null_SSE(1, 1, 2, 3, 0, 15, 13);
		round2_null_SSE(2, 1, 2, 3, 0, 15, 13);
		

		// Round 3 // --

		round3_SSE(0, 0, 3, 2, 1, 0, 3);
		round3_SSE(1, 0, 3, 2, 1, 0, 3);
		round3_SSE(2, 0, 3, 2, 1, 0, 3);
		round3_null_SSE(0, 3, 2, 1, 0, 8, 9);
		round3_null_SSE(1, 3, 2, 1, 0, 8, 9);
		round3_null_SSE(2, 3, 2, 1, 0, 8, 9);
		round3_SSE(0, 2, 1, 0, 3, 4, 11);
		round3_SSE(1, 2, 1, 0, 3, 4, 11);
		round3_SSE(2, 2, 1, 0, 3, 4, 11);
		round3_null_SSE(0, 1, 0, 3, 2, 12, 15);
		round3_null_SSE(1, 1, 0, 3, 2, 12, 15);
		round3_null_SSE(2, 1, 0, 3, 2, 12, 15);
		

		round3_SSE(0, 0, 3, 2, 1, 2, 3);
		round3_SSE(1, 0, 3, 2, 1, 2, 3);
		round3_SSE(2, 0, 3, 2, 1, 2, 3);
		round3_null_SSE(0, 3, 2, 1, 0, 10, 9);
		round3_null_SSE(1, 3, 2, 1, 0, 10, 9);
		round3_null_SSE(2, 3, 2, 1, 0, 10, 9);
		round3_SSE(0, 2, 1, 0, 3, 6, 11);
		round3_SSE(1, 2, 1, 0, 3, 6, 11);
		round3_SSE(2, 2, 1, 0, 3, 6, 11);
		round3_SSE(0, 1, 0, 3, 2, 14, 15);
		round3_SSE(1, 1, 0, 3, 2, 14, 15);
		round3_SSE(2, 1, 0, 3, 2, 14, 15);
		

		round3_SSE(0, 0, 3, 2, 1, 1, 3);
		round3_SSE(1, 0, 3, 2, 1, 1, 3);
		round3_SSE(2, 0, 3, 2, 1, 1, 3);
		round3_null_SSE(0, 3, 2, 1, 0, 9, 9);
		round3_null_SSE(1, 3, 2, 1, 0, 9, 9);
		round3_null_SSE(2, 3, 2, 1, 0, 9, 9);
		round3_SSE(0, 2, 1, 0, 3, 5, 11);
		round3_SSE(1, 2, 1, 0, 3, 5, 11);
		round3_SSE(2, 2, 1, 0, 3, 5, 11);
		round3_null_SSE(0, 1, 0, 3, 2, 13, 15);
		round3_null_SSE(1, 1, 0, 3, 2, 13, 15);
		round3_null_SSE(2, 1, 0, 3, 2, 13, 15);
		

		round3_SSE(0, 0, 3, 2, 1, 3, 3);
		round3_SSE(1, 0, 3, 2, 1, 3, 3);
		round3_SSE(2, 0, 3, 2, 1, 3, 3);
		round3_null_SSE(0, 3, 2, 1, 0, 11, 9);
		round3_null_SSE(1, 3, 2, 1, 0, 11, 9);
		round3_null_SSE(2, 3, 2, 1, 0, 11, 9);
		round3_SSE(0, 2, 1, 0, 3, 7, 11);
		round3_SSE(1, 2, 1, 0, 3, 7, 11);
		round3_SSE(2, 2, 1, 0, 3, 7, 11);
		round3_null_SSE(0, 1, 0, 3, 2, 15, 15);
		round3_null_SSE(1, 1, 0, 3, 2, 15, 15);
		round3_null_SSE(2, 1, 0, 3, 2, 15, 15);
		

		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 4; j++)
				_mm_store_si128((__m128i*)wd[i][j], wd_SSE[i][j]);
	}

	inline void convertToContext_SSE(hashContext* output)
	{
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 4; j++)
				for(int k = 0; k < 4; k++)
					output[j+4*i].wd[k] = wd[i][k][j];
	}

	__m128i message_SSE[3][16];
	__m128i wd_SSE[3][4];

	__m128i SQRT_2_sse, SQRT_3_sse;

	unsigned int message[16][4];
	unsigned int wd[3][4][4];
};

#endif