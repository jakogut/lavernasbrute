#ifndef NTLM_MD_
#define NTLM_MD_

#include "NTLM.h"
#include "OverloadSSE.h"

class NTLM_SSE2 : public NTLM
{
public:

	NTLM_SSE2()
	{
		SQRT_2_md = _mm_set1_epi32(0x5a827999);
		SQRT_3_md = _mm_set1_epi32(0x6ed9eba1);
	}

	~NTLM_SSE2()
	{
	}

	void getMultipleWeakHashes(std::string input[8], int64_pair output[8])
	{
		prepare_key_md(input);

		initialize_words_md();

		md4_crypt_md();

		convert_to_int128_md(output);
	}

protected:

	inline void prepare_key_md(std::string* input)
	{
		int length[2][4];

		for(int i = 0; i < 2; i++)
			for(int j = 0; j < 4; j++)
				length[i][j] = input[j+4*i].length();

		for(int i = 0; i < 2; i++)
		{
			memset(nt_buffer_md, 0, 16*4*4);

			for(int j = 0; j < 4; j++)
			{
				int k = 0;
				//The length of input need to be <= 27
				for(;k<length[i][j]/2;k++)	
					nt_buffer_md[k][j] = input[j+4*i].c_str()[2*k] | (input[j+4*i].c_str()[2*k+1]<<16);
			 
				//padding
				if(length[i][j]%2==1)
					nt_buffer_md[k][j] = input[j+4*i].c_str()[length[i][j]-1] | 0x800000;
				else
					nt_buffer_md[k][j]=0x80;
				//put the length
				nt_buffer_md[14][j] = length[i][j] << 4;
			}

			for(int j = 0; j < 16; j++)
				nt_buffer[i][j] = _mm_loadu_si128((__m128i*)nt_buffer_md[j]);
		}
	}

	inline void initialize_words_md()
	{
		for(int i = 0; i < 2; i++)
		{
			wd[i][0] = _mm_set1_epi32(0x67452301);
			wd[i][1] = _mm_set1_epi32(0xefcdab89);
			wd[i][2] = _mm_set1_epi32(0x98badcfe);
			wd[i][3] = _mm_set1_epi32(0x10325476);
		}
	}

	inline void md4_crypt_md()
	{	 
		wd[0][0] += (wd[0][3] ^ (wd[0][1] & (wd[0][2] ^ wd[0][3])))  +  nt_buffer[0][0];  wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += (wd[1][3] ^ (wd[1][1] & (wd[1][2] ^ wd[1][3])))  +  nt_buffer[1][0];  wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += (wd[0][2] ^ (wd[0][0] & (wd[0][1] ^ wd[0][2])))  +  nt_buffer[0][1];  wd[0][3] = ROTL_md(wd[0][3], 7);
		wd[1][3] += (wd[1][2] ^ (wd[1][0] & (wd[1][1] ^ wd[1][2])))  +  nt_buffer[1][1];  wd[1][3] = ROTL_md(wd[1][3], 7);
		wd[0][2] += (wd[0][1] ^ (wd[0][3] & (wd[0][0] ^ wd[0][1])))  +  nt_buffer[0][2];  wd[0][2] = ROTL_md(wd[0][2], 11);
		wd[1][2] += (wd[1][1] ^ (wd[1][3] & (wd[1][0] ^ wd[1][1])))  +  nt_buffer[1][2];  wd[1][2] = ROTL_md(wd[1][2], 11);
		wd[0][1] += (wd[0][0] ^ (wd[0][2] & (wd[0][3] ^ wd[0][0])))  +  nt_buffer[0][3];  wd[0][1] = ROTL_md(wd[0][1], 19);
		wd[1][1] += (wd[1][0] ^ (wd[1][2] & (wd[1][3] ^ wd[1][0])))  +  nt_buffer[1][3];  wd[1][1] = ROTL_md(wd[1][1], 19);
	 
		wd[0][0] += (wd[0][3] ^ (wd[0][1] & (wd[0][2] ^ wd[0][3])))  +  nt_buffer[0][4];  wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += (wd[1][3] ^ (wd[1][1] & (wd[1][2] ^ wd[1][3])))  +  nt_buffer[1][4];  wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += (wd[0][2] ^ (wd[0][0] & (wd[0][1] ^ wd[0][2])))  +  nt_buffer[0][5];  wd[0][3] = ROTL_md(wd[0][3], 7);
		wd[1][3] += (wd[1][2] ^ (wd[1][0] & (wd[1][1] ^ wd[1][2])))  +  nt_buffer[1][5];  wd[1][3] = ROTL_md(wd[1][3], 7);
		wd[0][2] += (wd[0][1] ^ (wd[0][3] & (wd[0][0] ^ wd[0][1])))  +  nt_buffer[0][6];  wd[0][2] = ROTL_md(wd[0][2], 11);
		wd[1][2] += (wd[1][1] ^ (wd[1][3] & (wd[1][0] ^ wd[1][1])))  +  nt_buffer[1][6];  wd[1][2] = ROTL_md(wd[1][2], 11);
		wd[0][1] += (wd[0][0] ^ (wd[0][2] & (wd[0][3] ^ wd[0][0])))  +  nt_buffer[0][7];  wd[0][1] = ROTL_md(wd[0][1], 19);
		wd[1][1] += (wd[1][0] ^ (wd[1][2] & (wd[1][3] ^ wd[1][0])))  +  nt_buffer[1][7];  wd[1][1] = ROTL_md(wd[1][1], 19);
	 
		wd[0][0] += (wd[0][3] ^ (wd[0][1] & (wd[0][2] ^ wd[0][3])))  +  nt_buffer[0][8];  wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += (wd[1][3] ^ (wd[1][1] & (wd[1][2] ^ wd[1][3])))  +  nt_buffer[1][8];  wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += (wd[0][2] ^ (wd[0][0] & (wd[0][1] ^ wd[0][2])))  +  nt_buffer[0][9];  wd[0][3] = ROTL_md(wd[0][3], 7);
		wd[1][3] += (wd[1][2] ^ (wd[1][0] & (wd[1][1] ^ wd[1][2])))  +  nt_buffer[1][9];  wd[1][3] = ROTL_md(wd[1][3], 7);
		wd[0][2] += (wd[0][1] ^ (wd[0][3] & (wd[0][0] ^ wd[0][1])))  +  nt_buffer[0][10];  wd[0][2] = ROTL_md(wd[0][2], 11);
		wd[1][2] += (wd[1][1] ^ (wd[1][3] & (wd[1][0] ^ wd[1][1])))  +  nt_buffer[1][10];  wd[1][2] = ROTL_md(wd[1][2], 11);
		wd[0][1] += (wd[0][0] ^ (wd[0][2] & (wd[0][3] ^ wd[0][0])))  +  nt_buffer[0][11];  wd[0][1] = ROTL_md(wd[0][1], 19);
		wd[1][1] += (wd[1][0] ^ (wd[1][2] & (wd[1][3] ^ wd[1][0])))  +  nt_buffer[1][11];  wd[1][1] = ROTL_md(wd[1][1], 19);

		wd[0][0] += (wd[0][3] ^ (wd[0][1] & (wd[0][2] ^ wd[0][3])))  +  nt_buffer[0][12];  wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += (wd[1][3] ^ (wd[1][1] & (wd[1][2] ^ wd[1][3])))  +  nt_buffer[1][12];  wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += (wd[0][2] ^ (wd[0][0] & (wd[0][1] ^ wd[0][2])))  +  nt_buffer[0][13];  wd[0][3] = ROTL_md(wd[0][3], 7);
		wd[1][3] += (wd[1][2] ^ (wd[1][0] & (wd[1][1] ^ wd[1][2])))  +  nt_buffer[1][13];  wd[1][3] = ROTL_md(wd[1][3], 7);
		wd[0][2] += (wd[0][1] ^ (wd[0][3] & (wd[0][0] ^ wd[0][1])))  +  nt_buffer[0][14];  wd[0][2] = ROTL_md(wd[0][2], 11);
		wd[1][2] += (wd[1][1] ^ (wd[1][3] & (wd[1][0] ^ wd[1][1])))  +  nt_buffer[1][14];  wd[1][2] = ROTL_md(wd[1][2], 11);
		wd[0][1] += (wd[0][0] ^ (wd[0][2] & (wd[0][3] ^ wd[0][0])))  +  nt_buffer[0][15];  wd[0][1] = ROTL_md(wd[0][1], 19);
		wd[1][1] += (wd[1][0] ^ (wd[1][2] & (wd[1][3] ^ wd[1][0])))  +  nt_buffer[1][15];  wd[1][1] = ROTL_md(wd[1][1], 19);

		wd[0][0] += ((wd[0][1] & (wd[0][2] | wd[0][3])) | (wd[0][2] & wd[0][3])) + nt_buffer[0][0] +SQRT_2_md; wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += ((wd[1][1] & (wd[1][2] | wd[1][3])) | (wd[1][2] & wd[1][3])) + nt_buffer[1][0] +SQRT_2_md; wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += ((wd[0][0] & (wd[0][1] | wd[0][2])) | (wd[0][1] & wd[0][2])) + nt_buffer[0][4] +SQRT_2_md; wd[0][3] = ROTL_md(wd[0][3], 5);
		wd[1][3] += ((wd[1][0] & (wd[1][1] | wd[1][2])) | (wd[1][1] & wd[1][2])) + nt_buffer[1][4] +SQRT_2_md; wd[1][3] = ROTL_md(wd[1][3], 5);
		wd[0][2] += ((wd[0][3] & (wd[0][0] | wd[0][1])) | (wd[0][0] & wd[0][1])) + nt_buffer[0][8] +SQRT_2_md; wd[0][2] = ROTL_md(wd[0][2], 9);
		wd[1][2] += ((wd[1][3] & (wd[1][0] | wd[1][1])) | (wd[1][0] & wd[1][1])) + nt_buffer[1][8] +SQRT_2_md; wd[1][2] = ROTL_md(wd[1][2], 9);
		wd[0][1] += ((wd[0][2] & (wd[0][3] | wd[0][0])) | (wd[0][3] & wd[0][0])) + nt_buffer[0][12]+SQRT_2_md; wd[0][1] = ROTL_md(wd[0][1], 13);
		wd[1][1] += ((wd[1][2] & (wd[1][3] | wd[1][0])) | (wd[1][3] & wd[1][0])) + nt_buffer[1][12]+SQRT_2_md; wd[1][1] = ROTL_md(wd[1][1], 13);
	 
		wd[0][0] += ((wd[0][1] & (wd[0][2] | wd[0][3])) | (wd[0][2] & wd[0][3])) + nt_buffer[0][1] +SQRT_2_md; wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += ((wd[1][1] & (wd[1][2] | wd[1][3])) | (wd[1][2] & wd[1][3])) + nt_buffer[1][1] +SQRT_2_md; wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += ((wd[0][0] & (wd[0][1] | wd[0][2])) | (wd[0][1] & wd[0][2])) + nt_buffer[0][5] +SQRT_2_md; wd[0][3] = ROTL_md(wd[0][3], 5);
		wd[1][3] += ((wd[1][0] & (wd[1][1] | wd[1][2])) | (wd[1][1] & wd[1][2])) + nt_buffer[1][5] +SQRT_2_md; wd[1][3] = ROTL_md(wd[1][3], 5);
		wd[0][2] += ((wd[0][3] & (wd[0][0] | wd[0][1])) | (wd[0][0] & wd[0][1])) + nt_buffer[0][9] +SQRT_2_md; wd[0][2] = ROTL_md(wd[0][2], 9);
		wd[1][2] += ((wd[1][3] & (wd[1][0] | wd[1][1])) | (wd[1][0] & wd[1][1])) + nt_buffer[1][9] +SQRT_2_md; wd[1][2] = ROTL_md(wd[1][2], 9);
		wd[0][1] += ((wd[0][2] & (wd[0][3] | wd[0][0])) | (wd[0][3] & wd[0][0])) + nt_buffer[0][13]+SQRT_2_md; wd[0][1] = ROTL_md(wd[0][1], 13);
		wd[1][1] += ((wd[1][2] & (wd[1][3] | wd[1][0])) | (wd[1][3] & wd[1][0])) + nt_buffer[1][13]+SQRT_2_md; wd[1][1] = ROTL_md(wd[1][1], 13);
	 
		wd[0][0] += ((wd[0][1] & (wd[0][2] | wd[0][3])) | (wd[0][2] & wd[0][3])) + nt_buffer[0][2] +SQRT_2_md; wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += ((wd[1][1] & (wd[1][2] | wd[1][3])) | (wd[1][2] & wd[1][3])) + nt_buffer[1][2] +SQRT_2_md; wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += ((wd[0][0] & (wd[0][1] | wd[0][2])) | (wd[0][1] & wd[0][2])) + nt_buffer[0][6] +SQRT_2_md; wd[0][3] = ROTL_md(wd[0][3], 5);
		wd[1][3] += ((wd[1][0] & (wd[1][1] | wd[1][2])) | (wd[1][1] & wd[1][2])) + nt_buffer[1][6] +SQRT_2_md; wd[1][3] = ROTL_md(wd[1][3], 5);
		wd[0][2] += ((wd[0][3] & (wd[0][0] | wd[0][1])) | (wd[0][0] & wd[0][1])) + nt_buffer[0][10]+SQRT_2_md; wd[0][2] = ROTL_md(wd[0][2], 9);
		wd[1][2] += ((wd[1][3] & (wd[1][0] | wd[1][1])) | (wd[1][0] & wd[1][1])) + nt_buffer[1][10]+SQRT_2_md; wd[1][2] = ROTL_md(wd[1][2], 9);
		wd[0][1] += ((wd[0][2] & (wd[0][3] | wd[0][0])) | (wd[0][3] & wd[0][0])) + nt_buffer[0][14]+SQRT_2_md; wd[0][1] = ROTL_md(wd[0][1], 13);
		wd[1][1] += ((wd[1][2] & (wd[1][3] | wd[1][0])) | (wd[1][3] & wd[1][0])) + nt_buffer[1][14]+SQRT_2_md; wd[1][1] = ROTL_md(wd[1][1], 13);
	 
		wd[0][0] += ((wd[0][1] & (wd[0][2] | wd[0][3])) | (wd[0][2] & wd[0][3])) + nt_buffer[0][3] +SQRT_2_md; wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += ((wd[1][1] & (wd[1][2] | wd[1][3])) | (wd[1][2] & wd[1][3])) + nt_buffer[1][3] +SQRT_2_md; wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += ((wd[0][0] & (wd[0][1] | wd[0][2])) | (wd[0][1] & wd[0][2])) + nt_buffer[0][7] +SQRT_2_md; wd[0][3] = ROTL_md(wd[0][3], 5);
		wd[1][3] += ((wd[1][0] & (wd[1][1] | wd[1][2])) | (wd[1][1] & wd[1][2])) + nt_buffer[1][7] +SQRT_2_md; wd[1][3] = ROTL_md(wd[1][3], 5);
		wd[0][2] += ((wd[0][3] & (wd[0][0] | wd[0][1])) | (wd[0][0] & wd[0][1])) + nt_buffer[0][11]+SQRT_2_md; wd[0][2] = ROTL_md(wd[0][2], 9);
		wd[1][2] += ((wd[1][3] & (wd[1][0] | wd[1][1])) | (wd[1][0] & wd[1][1])) + nt_buffer[1][11]+SQRT_2_md; wd[1][2] = ROTL_md(wd[1][2], 9);
		wd[0][1] += ((wd[0][2] & (wd[0][3] | wd[0][0])) | (wd[0][3] & wd[0][0])) + nt_buffer[0][15]+SQRT_2_md; wd[0][1] = ROTL_md(wd[0][1], 13);
		wd[1][1] += ((wd[1][2] & (wd[1][3] | wd[1][0])) | (wd[1][3] & wd[1][0])) + nt_buffer[1][15]+SQRT_2_md; wd[1][1] = ROTL_md(wd[1][1], 13);

		wd[0][0] += (wd[0][3] ^ wd[0][2] ^ wd[0][1]) + nt_buffer[0][0]  +  SQRT_3_md; wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += (wd[1][3] ^ wd[1][2] ^ wd[1][1]) + nt_buffer[1][0]  +  SQRT_3_md; wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += (wd[0][2] ^ wd[0][1] ^ wd[0][0]) + nt_buffer[0][8]  +  SQRT_3_md; wd[0][3] = ROTL_md(wd[0][3], 9);
		wd[1][3] += (wd[1][2] ^ wd[1][1] ^ wd[1][0]) + nt_buffer[1][8]  +  SQRT_3_md; wd[1][3] = ROTL_md(wd[1][3], 9);
		wd[0][2] += (wd[0][1] ^ wd[0][0] ^ wd[0][3]) + nt_buffer[0][4]  +  SQRT_3_md; wd[0][2] = ROTL_md(wd[0][2], 11);
		wd[1][2] += (wd[1][1] ^ wd[1][0] ^ wd[1][3]) + nt_buffer[1][4]  +  SQRT_3_md; wd[1][2] = ROTL_md(wd[1][2], 11);
		wd[0][1] += (wd[0][0] ^ wd[0][3] ^ wd[0][2]) + nt_buffer[0][12] +  SQRT_3_md; wd[0][1] = ROTL_md(wd[0][1], 15);
		wd[1][1] += (wd[1][0] ^ wd[1][3] ^ wd[1][2]) + nt_buffer[1][12] +  SQRT_3_md; wd[1][1] = ROTL_md(wd[1][1], 15);
	 
		wd[0][0] += (wd[0][3] ^ wd[0][2] ^ wd[0][1]) + nt_buffer[0][2]  +  SQRT_3_md; wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += (wd[1][3] ^ wd[1][2] ^ wd[1][1]) + nt_buffer[1][2]  +  SQRT_3_md; wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += (wd[0][2] ^ wd[0][1] ^ wd[0][0]) + nt_buffer[0][10] +  SQRT_3_md; wd[0][3] = ROTL_md(wd[0][3], 9);
		wd[1][3] += (wd[1][2] ^ wd[1][1] ^ wd[1][0]) + nt_buffer[1][10] +  SQRT_3_md; wd[1][3] = ROTL_md(wd[1][3], 9);
		wd[0][2] += (wd[0][1] ^ wd[0][0] ^ wd[0][3]) + nt_buffer[0][6]  +  SQRT_3_md; wd[0][2] = ROTL_md(wd[0][2], 11);
		wd[1][2] += (wd[1][1] ^ wd[1][0] ^ wd[1][3]) + nt_buffer[1][6]  +  SQRT_3_md; wd[1][2] = ROTL_md(wd[1][2], 11);
		wd[0][1] += (wd[0][0] ^ wd[0][3] ^ wd[0][2]) + nt_buffer[0][14] +  SQRT_3_md; wd[0][1] = ROTL_md(wd[0][1], 15);
		wd[1][1] += (wd[1][0] ^ wd[1][3] ^ wd[1][2]) + nt_buffer[1][14] +  SQRT_3_md; wd[1][1] = ROTL_md(wd[1][1], 15);
	 
		wd[0][0] += (wd[0][3] ^ wd[0][2] ^ wd[0][1]) + nt_buffer[0][1]  +  SQRT_3_md; wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += (wd[1][3] ^ wd[1][2] ^ wd[1][1]) + nt_buffer[1][1]  +  SQRT_3_md; wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += (wd[0][2] ^ wd[0][1] ^ wd[0][0]) + nt_buffer[0][9]  +  SQRT_3_md; wd[0][3] = ROTL_md(wd[0][3], 9);
		wd[1][3] += (wd[1][2] ^ wd[1][1] ^ wd[1][0]) + nt_buffer[1][9]  +  SQRT_3_md; wd[1][3] = ROTL_md(wd[1][3], 9);
		wd[0][2] += (wd[0][1] ^ wd[0][0] ^ wd[0][3]) + nt_buffer[0][5]  +  SQRT_3_md; wd[0][2] = ROTL_md(wd[0][2], 11);
		wd[1][2] += (wd[1][1] ^ wd[1][0] ^ wd[1][3]) + nt_buffer[1][5]  +  SQRT_3_md; wd[1][2] = ROTL_md(wd[1][2], 11);
		wd[0][1] += (wd[0][0] ^ wd[0][3] ^ wd[0][2]) + nt_buffer[0][13] +  SQRT_3_md; wd[0][1] = ROTL_md(wd[0][1], 15);
		wd[1][1] += (wd[1][0] ^ wd[1][3] ^ wd[1][2]) + nt_buffer[1][13] +  SQRT_3_md; wd[1][1] = ROTL_md(wd[1][1], 15);
	 
		wd[0][0] += (wd[0][3] ^ wd[0][2] ^ wd[0][1]) + nt_buffer[0][3]  +  SQRT_3_md; wd[0][0] = ROTL_md(wd[0][0], 3);
		wd[1][0] += (wd[1][3] ^ wd[1][2] ^ wd[1][1]) + nt_buffer[1][3]  +  SQRT_3_md; wd[1][0] = ROTL_md(wd[1][0], 3);
		wd[0][3] += (wd[0][2] ^ wd[0][1] ^ wd[0][0]) + nt_buffer[0][11] +  SQRT_3_md; wd[0][3] = ROTL_md(wd[0][3], 9);
		wd[1][3] += (wd[1][2] ^ wd[1][1] ^ wd[1][0]) + nt_buffer[1][11] +  SQRT_3_md; wd[1][3] = ROTL_md(wd[1][3], 9);
		wd[0][2] += (wd[0][1] ^ wd[0][0] ^ wd[0][3]) + nt_buffer[0][7]  +  SQRT_3_md; wd[0][2] = ROTL_md(wd[0][2], 11);
		wd[1][2] += (wd[1][1] ^ wd[1][0] ^ wd[1][3]) + nt_buffer[1][7]  +  SQRT_3_md; wd[1][2] = ROTL_md(wd[1][2], 11);
		wd[0][1] += (wd[0][0] ^ wd[0][3] ^ wd[0][2]) + nt_buffer[0][15] +  SQRT_3_md; wd[0][1] = ROTL_md(wd[0][1], 15);
		wd[1][1] += (wd[1][0] ^ wd[1][3] ^ wd[1][2]) + nt_buffer[1][15] +  SQRT_3_md; wd[1][1] = ROTL_md(wd[1][1], 15);

		for(int i = 0; i < 2; i++)
			for(int j = 0; j < 4; j++)
				_mm_store_si128((__m128i*)wd_md[i][j], wd[i][j]);
	}

	inline void convert_to_int128_md(int64_pair* output)
	{
		for(int i = 0; i < 2; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				output[j+4*i].first = (wd_md[i][0][j] << 31) | wd_md[i][1][j];
				output[j+4*i].second = (wd_md[i][0][j] << 31) | wd_md[i][3][j];
			}
		}
	}

	// The 'md' suffix stands for "multiple data"
	__m128i nt_buffer[2][16];
	unsigned int nt_buffer_md[16][4];

	__m128i wd[2][4];
	unsigned int wd_md[2][4][4];

	__m128i SQRT_2_md, SQRT_3_md;
};

#endif