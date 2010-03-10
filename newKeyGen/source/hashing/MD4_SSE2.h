#ifndef MD4_SSE2_H_
#define MD4_SSE2_H_

#include <emmintrin.h>

#include "MD4.h"
#include "KeyGenerator.h"

#define round1_sse2(set, wd_index, a, b, c, ntb_index, rotation) \
	wd[set][wd_index] = _mm_add_epi32(wd[set][wd_index], _mm_add_epi32(_mm_or_si128(_mm_and_si128(wd[set][a], wd[set][b]), _mm_andnot_si128(wd[set][a], wd[set][c])), md4_buffer[set][ntb_index])), \
	wd[set][wd_index] = ROTL_sse2(wd[set][wd_index], rotation)

#define round2_sse2(set, wd_index, a, b, c, ntb_index, rotation) \
	wd[set][wd_index] = _mm_add_epi32(wd[set][wd_index], _mm_add_epi32(_mm_add_epi32(_mm_or_si128(_mm_and_si128(wd[set][a], _mm_or_si128(wd[set][b], wd[set][c])), _mm_and_si128(wd[set][b], wd[set][c])), md4_buffer[set][ntb_index]), SQRT_2)), \
	wd[set][wd_index] = ROTL_sse2(wd[set][wd_index], rotation)

#define round3_sse2(set, wd_index, a, b, c, ntb_index, rotation) \
	wd[set][wd_index] = _mm_add_epi32(_mm_add_epi32(_mm_add_epi32(wd[set][wd_index], _mm_xor_si128(wd[set][a], _mm_xor_si128(wd[set][b], wd[set][c]))), md4_buffer[set][ntb_index]), SQRT_3), \
	wd[set][wd_index] = ROTL_sse2(wd[set][wd_index], rotation)

#define ROTL_sse2(num, places) (_mm_or_si128(_mm_slli_epi32(num, places), _mm_srli_epi32(num, (32 - places))))

class MD4_SSE2 : public MD4
{
public:

	MD4_SSE2()
	{
		SQRT_2 = _mm_set1_epi32(0x5a827999);
		SQRT_3 = _mm_set1_epi32(0x6ed9eba1);
	}

	~MD4_SSE2()
	{
	}

	// Not quite done yet
	/*void getMultipleWeakMD4Hashes(std::string input[12], int64_pair output[12])
	{
		prepare_key_md4_sse2(input);

		initialize_words_sse2();

		md4_crypt_sse2();

		convert_to_int128_sse2(output);
	}*/

	void getMultipleWeakNTLMHashes(std::string input[12], int64_pair output[12])
	{
		prepare_key_ntlm_sse2(input);

		initialize_words_sse2();

		md4_crypt_sse2();

		convert_to_int128_sse2(output);
	}

protected:

	/*inline void prepare_key_md4_sse2(std::string* input)
	{
		unsigned int length[3][4];

		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 4; j++)
				length[i][j] = (unsigned int)input[j+4*i].length();

		for(int i = 0; i < 3; i++)
		{
			memset(md4_buffer_sse2,0,16*4*4);

			for(int j = 0; j < 4; j++)
			{
				unsigned int k=0;
				//The length of input need to be <= 27
				for(;k<length[i][j]/4;k++)	
					md4_buffer_sse2[k][j] = input[j+4*i][4*k] | (input[j+4*i][4*k+1]<<8) | (input[j+4*i][4*k+2]<<16) | (input[j+4*i][4*k+3]<<24);
			 
				//padding
				switch(length[i][j]%4)
				{
				case 0:
					md4_buffer_sse2[k][j] = 0x80;
					break;
				case 1:
					md4_buffer_sse2[k][j] = input[j+4*i][length[i][j]-1] | 0x8000;
					break;
				case 2:
					md4_buffer_sse2[k][j] = input[j+4*i][length[i][j]-2] | (input[j+4*i][length[i][j]-1]<<8) | 0x800000;
					break;
				case 3:
					md4_buffer_sse2[k][j] = input[j+4*i][length[i][j]-3] | (input[j+4*i][length[i][j]-2]<<8) | (input[j+4*i][length[i][j]-1]<<16) | 0x80000000;
					break;
				}

				//put the length
				md4_buffer_sse2[14][j] = length[i][j] << 3;
			}

			for(int j = 0; j < 3; j++)
				md4_buffer[i][j] = _mm_load_si128((__m128i*)md4_buffer_sse2[j]);
		}
	}*/

	inline void prepare_key_ntlm_sse2(std::string* input)
	{
		unsigned int length[3][4];

		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 4; j++)
				length[i][j] = (unsigned int)input[j+4*i].length();

		for(int i = 0; i < 3; i++)
		{
			memset(md4_buffer_sse2, 0, 16*4*4);

			for(int j = 0; j < 4; j++)
			{
				unsigned int k = 0;
				//The length of input need to be <= 27
				for(;k<length[i][j]/2;k++)	
					md4_buffer_sse2[k][j] = input[j+4*i][2*k] | (input[j+4*i][2*k+1]<<16);
			 
				//padding
				if(length[i][j]%2==1)
					md4_buffer_sse2[k][j] = input[j+4*i][length[i][j]-1] | 0x800000;
				else
					md4_buffer_sse2[k][j]=0x80;
				//put the length
				md4_buffer_sse2[14][j] = length[i][j] << 4;
			}

			for(int j = 0; j < 16; j++)
				md4_buffer[i][j] = _mm_load_si128((__m128i*)md4_buffer_sse2[j]);
		}
	}

	inline void initialize_words_sse2()
	{
		for(int i = 0; i < 3; i++)
		{
			wd[i][0] = _mm_set1_epi32(wd_init[0]);
			wd[i][1] = _mm_set1_epi32(wd_init[1]);
			wd[i][2] = _mm_set1_epi32(wd_init[2]);
			wd[i][3] = _mm_set1_epi32(wd_init[3]);
		}
	}

	inline void md4_crypt_sse2()
	{	 
		// Round 1 // ---

		round1_sse2(0, 0, 1, 2, 3, 0, 3);
		round1_sse2(1, 0, 1, 2, 3, 0, 3);
		round1_sse2(2, 0, 1, 2, 3, 0, 3);
		round1_sse2(0, 3, 0, 1, 2, 1, 7);
		round1_sse2(1, 3, 0, 1, 2, 1, 7);
		round1_sse2(2, 3, 0, 1, 2, 1, 7);
		round1_sse2(0, 2, 3, 0, 1, 2, 11);
		round1_sse2(1, 2, 3, 0, 1, 2, 11);
		round1_sse2(2, 2, 3, 0, 1, 2, 11);
		round1_sse2(0, 1, 2, 3, 0, 3, 19);
		round1_sse2(1, 1, 2, 3, 0, 3, 19);
		round1_sse2(2, 1, 2, 3, 0, 3, 19);
		

		round1_sse2(0, 0, 1, 2, 3, 4, 3);
		round1_sse2(1, 0, 1, 2, 3, 4, 3);
		round1_sse2(2, 0, 1, 2, 3, 4, 3);
		round1_sse2(0, 3, 0, 1, 2, 5, 7);
		round1_sse2(1, 3, 0, 1, 2, 5, 7);
		round1_sse2(2, 3, 0, 1, 2, 5, 7);
		round1_sse2(0, 2, 3, 0, 1, 6, 11);
		round1_sse2(1, 2, 3, 0, 1, 6, 11);
		round1_sse2(2, 2, 3, 0, 1, 6, 11);
		round1_sse2(0, 1, 2, 3, 0, 7, 19);
		round1_sse2(1, 1, 2, 3, 0, 7, 19);
		round1_sse2(2, 1, 2, 3, 0, 7, 19);
		

		round1_sse2(0, 0, 1, 2, 3, 8, 3);
		round1_sse2(1, 0, 1, 2, 3, 8, 3);
		round1_sse2(2, 0, 1, 2, 3, 8, 3);
		round1_sse2(0, 3, 0, 1, 2, 9, 7);
		round1_sse2(1, 3, 0, 1, 2, 9, 7);
		round1_sse2(2, 3, 0, 1, 2, 9, 7);
		round1_sse2(0, 2, 3, 0, 1, 10, 11);
		round1_sse2(1, 2, 3, 0, 1, 10, 11);
		round1_sse2(2, 2, 3, 0, 1, 10, 11);
		round1_sse2(0, 1, 2, 3, 0, 11, 19);
		round1_sse2(1, 1, 2, 3, 0, 11, 19);
		round1_sse2(2, 1, 2, 3, 0, 11, 19);
		

		round1_sse2(0, 0, 1, 2, 3, 12, 3);
		round1_sse2(1, 0, 1, 2, 3, 12, 3);
		round1_sse2(2, 0, 1, 2, 3, 12, 3);
		round1_sse2(0, 3, 0, 1, 2, 13, 7);
		round1_sse2(1, 3, 0, 1, 2, 13, 7);
		round1_sse2(2, 3, 0, 1, 2, 13, 7);
		round1_sse2(0, 2, 3, 0, 1, 14, 11);
		round1_sse2(1, 2, 3, 0, 1, 14, 11);
		round1_sse2(2, 2, 3, 0, 1, 14, 11);
		round1_sse2(0, 1, 2, 3, 0, 15, 19);
		round1_sse2(1, 1, 2, 3, 0, 15, 19);
		round1_sse2(2, 1, 2, 3, 0, 15, 19);
		

		// Round 2 // ---

		round2_sse2(0, 0, 1, 2, 3, 0, 3);
		round2_sse2(1, 0, 1, 2, 3, 0, 3);
		round2_sse2(2, 0, 1, 2, 3, 0, 3);
		round2_sse2(0, 3, 0, 1, 2, 4, 5);
		round2_sse2(1, 3, 0, 1, 2, 4, 5);
		round2_sse2(2, 3, 0, 1, 2, 4, 5);
		round2_sse2(0, 2, 3, 0, 1, 8, 9);
		round2_sse2(1, 2, 3, 0, 1, 8, 9);
		round2_sse2(2, 2, 3, 0, 1, 8, 9);
		round2_sse2(0, 1, 2, 3, 0, 12, 13);
		round2_sse2(1, 1, 2, 3, 0, 12, 13);
		round2_sse2(2, 1, 2, 3, 0, 12, 13);
		

		round2_sse2(0, 0, 1, 2, 3, 1, 3);
		round2_sse2(1, 0, 1, 2, 3, 1, 3);
		round2_sse2(2, 0, 1, 2, 3, 1, 3);
		round2_sse2(0, 3, 0, 1, 2, 5, 5);
		round2_sse2(1, 3, 0, 1, 2, 5, 5);
		round2_sse2(2, 3, 0, 1, 2, 5, 5);
		round2_sse2(0, 2, 3, 0, 1, 9, 9);
		round2_sse2(1, 2, 3, 0, 1, 9, 9);
		round2_sse2(2, 2, 3, 0, 1, 9, 9);
		round2_sse2(0, 1, 2, 3, 0, 13, 13);
		round2_sse2(1, 1, 2, 3, 0, 13, 13);
		round2_sse2(2, 1, 2, 3, 0, 13, 13);
		

		round2_sse2(0, 0, 1, 2, 3, 2, 3);
		round2_sse2(1, 0, 1, 2, 3, 2, 3);
		round2_sse2(2, 0, 1, 2, 3, 2, 3);
		round2_sse2(0, 3, 0, 1, 2, 6, 5);
		round2_sse2(1, 3, 0, 1, 2, 6, 5);
		round2_sse2(2, 3, 0, 1, 2, 6, 5);
		round2_sse2(0, 2, 3, 0, 1, 10, 9);
		round2_sse2(1, 2, 3, 0, 1, 10, 9);
		round2_sse2(2, 2, 3, 0, 1, 10, 9);
		round2_sse2(0, 1, 2, 3, 0, 14, 13);
		round2_sse2(1, 1, 2, 3, 0, 14, 13);
		round2_sse2(2, 1, 2, 3, 0, 14, 13);
		

		round2_sse2(0, 0, 1, 2, 3, 3, 3);
		round2_sse2(1, 0, 1, 2, 3, 3, 3);
		round2_sse2(2, 0, 1, 2, 3, 3, 3);
		round2_sse2(0, 3, 0, 1, 2, 7, 5);
		round2_sse2(1, 3, 0, 1, 2, 7, 5);
		round2_sse2(2, 3, 0, 1, 2, 7, 5);
		round2_sse2(0, 2, 3, 0, 1, 11, 9);
		round2_sse2(1, 2, 3, 0, 1, 11, 9);
		round2_sse2(2, 2, 3, 0, 1, 11, 9);
		round2_sse2(0, 1, 2, 3, 0, 15, 13);
		round2_sse2(1, 1, 2, 3, 0, 15, 13);
		round2_sse2(2, 1, 2, 3, 0, 15, 13);
		

		// Round 3 // --

		round3_sse2(0, 0, 3, 2, 1, 0, 3);
		round3_sse2(1, 0, 3, 2, 1, 0, 3);
		round3_sse2(2, 0, 3, 2, 1, 0, 3);
		round3_sse2(0, 3, 2, 1, 0, 8, 9);
		round3_sse2(1, 3, 2, 1, 0, 8, 9);
		round3_sse2(2, 3, 2, 1, 0, 8, 9);
		round3_sse2(0, 2, 1, 0, 3, 4, 11);
		round3_sse2(1, 2, 1, 0, 3, 4, 11);
		round3_sse2(2, 2, 1, 0, 3, 4, 11);
		round3_sse2(0, 1, 0, 3, 2, 12, 15);
		round3_sse2(1, 1, 0, 3, 2, 12, 15);
		round3_sse2(2, 1, 0, 3, 2, 12, 15);
		

		round3_sse2(0, 0, 3, 2, 1, 2, 3);
		round3_sse2(1, 0, 3, 2, 1, 2, 3);
		round3_sse2(2, 0, 3, 2, 1, 2, 3);
		round3_sse2(0, 3, 2, 1, 0, 10, 9);
		round3_sse2(1, 3, 2, 1, 0, 10, 9);
		round3_sse2(2, 3, 2, 1, 0, 10, 9);
		round3_sse2(0, 2, 1, 0, 3, 6, 11);
		round3_sse2(1, 2, 1, 0, 3, 6, 11);
		round3_sse2(2, 2, 1, 0, 3, 6, 11);
		round3_sse2(0, 1, 0, 3, 2, 14, 15);
		round3_sse2(1, 1, 0, 3, 2, 14, 15);
		round3_sse2(2, 1, 0, 3, 2, 14, 15);
		

		round3_sse2(0, 0, 3, 2, 1, 1, 3);
		round3_sse2(1, 0, 3, 2, 1, 1, 3);
		round3_sse2(2, 0, 3, 2, 1, 1, 3);
		round3_sse2(0, 3, 2, 1, 0, 9, 9);
		round3_sse2(1, 3, 2, 1, 0, 9, 9);
		round3_sse2(2, 3, 2, 1, 0, 9, 9);
		round3_sse2(0, 2, 1, 0, 3, 5, 11);
		round3_sse2(1, 2, 1, 0, 3, 5, 11);
		round3_sse2(2, 2, 1, 0, 3, 5, 11);
		round3_sse2(0, 1, 0, 3, 2, 13, 15);
		round3_sse2(1, 1, 0, 3, 2, 13, 15);
		round3_sse2(2, 1, 0, 3, 2, 13, 15);
		

		round3_sse2(0, 0, 3, 2, 1, 3, 3);
		round3_sse2(1, 0, 3, 2, 1, 3, 3);
		round3_sse2(2, 0, 3, 2, 1, 3, 3);
		round3_sse2(0, 3, 2, 1, 0, 11, 9);
		round3_sse2(1, 3, 2, 1, 0, 11, 9);
		round3_sse2(2, 3, 2, 1, 0, 11, 9);
		round3_sse2(0, 2, 1, 0, 3, 7, 11);
		round3_sse2(1, 2, 1, 0, 3, 7, 11);
		round3_sse2(2, 2, 1, 0, 3, 7, 11);
		round3_sse2(0, 1, 0, 3, 2, 15, 15);
		round3_sse2(1, 1, 0, 3, 2, 15, 15);
		round3_sse2(2, 1, 0, 3, 2, 15, 15);
		

		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 4; j++)
				_mm_store_si128((__m128i*)wd_sse2[i][j], wd[i][j]);
	}

	inline void convert_to_int128_sse2(int64_pair* output)
	{
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				output[j+4*i].first  = ((unsigned long long)wd_sse2[i][0][j] << 32) | wd_sse2[i][2][j];
				output[j+4*i].second = ((unsigned long long)wd_sse2[i][1][j] << 32) | wd_sse2[i][3][j];
			}
		}
	}

	__m128i md4_buffer[3][16];
	__m128i wd[3][4];

	__m128i SQRT_2, SQRT_3;

	// The 'md' suffix stands for "multiple data"
	unsigned int md4_buffer_sse2[16][4];
	unsigned int wd_sse2[3][4][4];
};

#endif
