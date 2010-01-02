#ifndef NTLM_MD_
#define NTLM_MD_

#include "NTLM.h"
#include "OverloadSSE.h"

class NTLM_SSE2 : public NTLM
{
public:

	NTLM_SSE2()
	{
	}

	~NTLM_SSE2()
	{
	}

	void getMultipleHashes(std::string input[4], std::string output[4])
	{
		prepare_key_md(input);

		initialize_words_md();

		md4_crypt_round1_md();
		md4_crypt_round2_md();
		md4_crypt_round3_md();

		finalize_md4_md();

		convert_to_hex_md(output);
	}

	void getMultipleWeakHashes(std::string input[4], int64_pair output[4])
	{
		prepare_key_md(input);

		initialize_words_md();

		md4_crypt_round1_md();
		md4_crypt_round2_md();
		md4_crypt_round3_md();

		finalize_md4();

		convert_to_int128_md();
	}

protected:

	// Rewrite this in SSE2
	void prepare_key_md(std::string input[4])
	{
		int length[4];

		for(int i = 0; i < 4; i++)
			length[i] = input[i].length();

		memset(nt_buffer_sse, 0, 16*4*4);

		for(int i = 0; i < 4; i++)
		{
			int j = 0;
			//The length of input need to be <= 27
			for(;j<length[i]/2;j++)	
				nt_buffer_md[j][i] = input[i].c_str()[2*j] | (input[i].c_str()[2*j+1]<<16);
		 
			//padding
			if(length[i]%2==1)
				nt_buffer_md[j][i] = input[i].c_str()[length[i]-1] | 0x800000;
			else
				nt_buffer_md[j][i]=0x80;
			//put the length
			nt_buffer_md[14][i] = length[i] << 4;
		}

		for(int i = 0; i < 16; i++)
			memcpy(&nt_buffer_sse[i], nt_buffer_md[i], (sizeof(unsigned int) * 4));
	}

	void initialize_words_md()
	{
		SQRT_2_sse = _mm_set1_epi32(0x5a827999);
		SQRT_3_sse = _mm_set1_epi32(0x6ed9eba1);

		wd_sse[0] = _mm_set1_epi32(0x67452301);
		wd_sse[1] = _mm_set1_epi32(0xefcdab89);
		wd_sse[2] = _mm_set1_epi32(0x98badcfe);
		wd_sse[3] = _mm_set1_epi32(0x10325476);
	}

	void md4_crypt_round1_md()
	{	 
		wd_sse[0] += (wd_sse[3] ^ (wd_sse[1] & (wd_sse[2] ^ wd_sse[3])))  +  nt_buffer_sse[0];  wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += (wd_sse[2] ^ (wd_sse[0] & (wd_sse[1] ^ wd_sse[2])))  +  nt_buffer_sse[1];  wd_sse[3] = ROTL_md(wd_sse[3], 7);
		wd_sse[2] += (wd_sse[1] ^ (wd_sse[3] & (wd_sse[0] ^ wd_sse[1])))  +  nt_buffer_sse[2];  wd_sse[2] = ROTL_md(wd_sse[2], 11);
		wd_sse[1] += (wd_sse[0] ^ (wd_sse[2] & (wd_sse[3] ^ wd_sse[0])))  +  nt_buffer_sse[3];  wd_sse[1] = ROTL_md(wd_sse[1], 19);
	 
		wd_sse[0] += (wd_sse[3] ^ (wd_sse[1] & (wd_sse[2] ^ wd_sse[3])))  +  nt_buffer_sse[4];  wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += (wd_sse[2] ^ (wd_sse[0] & (wd_sse[1] ^ wd_sse[2])))  +  nt_buffer_sse[5];  wd_sse[3] = ROTL_md(wd_sse[3], 7);
		wd_sse[2] += (wd_sse[1] ^ (wd_sse[3] & (wd_sse[0] ^ wd_sse[1])))  +  nt_buffer_sse[6];  wd_sse[2] = ROTL_md(wd_sse[2], 11);
		wd_sse[1] += (wd_sse[0] ^ (wd_sse[2] & (wd_sse[3] ^ wd_sse[0])))  +  nt_buffer_sse[7];  wd_sse[1] = ROTL_md(wd_sse[1], 19);
	 
		wd_sse[0] += (wd_sse[3] ^ (wd_sse[1] & (wd_sse[2] ^ wd_sse[3])))  +  nt_buffer_sse[8];  wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += (wd_sse[2] ^ (wd_sse[0] & (wd_sse[1] ^ wd_sse[2])))  +  nt_buffer_sse[9];  wd_sse[3] = ROTL_md(wd_sse[3], 7);
		wd_sse[2] += (wd_sse[1] ^ (wd_sse[3] & (wd_sse[0] ^ wd_sse[1])))  +  nt_buffer_sse[10]; wd_sse[2] = ROTL_md(wd_sse[2], 11);
		wd_sse[1] += (wd_sse[0] ^ (wd_sse[2] & (wd_sse[3] ^ wd_sse[0])))  +  nt_buffer_sse[11]; wd_sse[1] = ROTL_md(wd_sse[1], 19);
	 
		wd_sse[0] += (wd_sse[3] ^ (wd_sse[1] & (wd_sse[2] ^ wd_sse[3])))  +  nt_buffer_sse[12]; wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += (wd_sse[2] ^ (wd_sse[0] & (wd_sse[1] ^ wd_sse[2])))  +  nt_buffer_sse[13]; wd_sse[3] = ROTL_md(wd_sse[3], 7);
		wd_sse[2] += (wd_sse[1] ^ (wd_sse[3] & (wd_sse[0] ^ wd_sse[1])))  +  nt_buffer_sse[14]; wd_sse[2] = ROTL_md(wd_sse[2], 11);
		wd_sse[1] += (wd_sse[0] ^ (wd_sse[2] & (wd_sse[3] ^ wd_sse[0])))  +  nt_buffer_sse[15]; wd_sse[1] = ROTL_md(wd_sse[1], 19);
	}
	 
	void md4_crypt_round2_md()
	{
		wd_sse[0] += ((wd_sse[1] & (wd_sse[2] | wd_sse[3])) | (wd_sse[2] & wd_sse[3])) + nt_buffer_sse[0] +SQRT_2_sse; wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += ((wd_sse[0] & (wd_sse[1] | wd_sse[2])) | (wd_sse[1] & wd_sse[2])) + nt_buffer_sse[4] +SQRT_2_sse; wd_sse[3] = ROTL_md(wd_sse[3], 5);
		wd_sse[2] += ((wd_sse[3] & (wd_sse[0] | wd_sse[1])) | (wd_sse[0] & wd_sse[1])) + nt_buffer_sse[8] +SQRT_2_sse; wd_sse[2] = ROTL_md(wd_sse[2], 9);
		wd_sse[1] += ((wd_sse[2] & (wd_sse[3] | wd_sse[0])) | (wd_sse[3] & wd_sse[0])) + nt_buffer_sse[12]+SQRT_2_sse; wd_sse[1] = ROTL_md(wd_sse[1], 13);
	 
		wd_sse[0] += ((wd_sse[1] & (wd_sse[2] | wd_sse[3])) | (wd_sse[2] & wd_sse[3])) + nt_buffer_sse[1] +SQRT_2_sse; wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += ((wd_sse[0] & (wd_sse[1] | wd_sse[2])) | (wd_sse[1] & wd_sse[2])) + nt_buffer_sse[5] +SQRT_2_sse; wd_sse[3] = ROTL_md(wd_sse[3], 5);
		wd_sse[2] += ((wd_sse[3] & (wd_sse[0] | wd_sse[1])) | (wd_sse[0] & wd_sse[1])) + nt_buffer_sse[9] +SQRT_2_sse; wd_sse[2] = ROTL_md(wd_sse[2], 9);
		wd_sse[1] += ((wd_sse[2] & (wd_sse[3] | wd_sse[0])) | (wd_sse[3] & wd_sse[0])) + nt_buffer_sse[13]+SQRT_2_sse; wd_sse[1] = ROTL_md(wd_sse[1], 13);
	 
		wd_sse[0] += ((wd_sse[1] & (wd_sse[2] | wd_sse[3])) | (wd_sse[2] & wd_sse[3])) + nt_buffer_sse[2] +SQRT_2_sse; wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += ((wd_sse[0] & (wd_sse[1] | wd_sse[2])) | (wd_sse[1] & wd_sse[2])) + nt_buffer_sse[6] +SQRT_2_sse; wd_sse[3] = ROTL_md(wd_sse[3], 5);
		wd_sse[2] += ((wd_sse[3] & (wd_sse[0] | wd_sse[1])) | (wd_sse[0] & wd_sse[1])) + nt_buffer_sse[10]+SQRT_2_sse; wd_sse[2] = ROTL_md(wd_sse[2], 9);
		wd_sse[1] += ((wd_sse[2] & (wd_sse[3] | wd_sse[0])) | (wd_sse[3] & wd_sse[0])) + nt_buffer_sse[14]+SQRT_2_sse; wd_sse[1] = ROTL_md(wd_sse[1], 13);
	 
		wd_sse[0] += ((wd_sse[1] & (wd_sse[2] | wd_sse[3])) | (wd_sse[2] & wd_sse[3])) + nt_buffer_sse[3] +SQRT_2_sse; wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += ((wd_sse[0] & (wd_sse[1] | wd_sse[2])) | (wd_sse[1] & wd_sse[2])) + nt_buffer_sse[7] +SQRT_2_sse; wd_sse[3] = ROTL_md(wd_sse[3], 5);
		wd_sse[2] += ((wd_sse[3] & (wd_sse[0] | wd_sse[1])) | (wd_sse[0] & wd_sse[1])) + nt_buffer_sse[11]+SQRT_2_sse; wd_sse[2] = ROTL_md(wd_sse[2], 9);
		wd_sse[1] += ((wd_sse[2] & (wd_sse[3] | wd_sse[0])) | (wd_sse[3] & wd_sse[0])) + nt_buffer_sse[15]+SQRT_2_sse; wd_sse[1] = ROTL_md(wd_sse[1], 13);
	}
	 
	void md4_crypt_round3_md()
	{
		wd_sse[0] += (wd_sse[3] ^ wd_sse[2] ^ wd_sse[1]) + nt_buffer_sse[0]  +  SQRT_3_sse; wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += (wd_sse[2] ^ wd_sse[1] ^ wd_sse[0]) + nt_buffer_sse[8]  +  SQRT_3_sse; wd_sse[3] = ROTL_md(wd_sse[3], 9);
		wd_sse[2] += (wd_sse[1] ^ wd_sse[0] ^ wd_sse[3]) + nt_buffer_sse[4]  +  SQRT_3_sse; wd_sse[2] = ROTL_md(wd_sse[2], 11);
		wd_sse[1] += (wd_sse[0] ^ wd_sse[3] ^ wd_sse[2]) + nt_buffer_sse[12] +  SQRT_3_sse; wd_sse[1] = ROTL_md(wd_sse[1], 15);
	 
		wd_sse[0] += (wd_sse[3] ^ wd_sse[2] ^ wd_sse[1]) + nt_buffer_sse[2]  +  SQRT_3_sse; wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += (wd_sse[2] ^ wd_sse[1] ^ wd_sse[0]) + nt_buffer_sse[10] +  SQRT_3_sse; wd_sse[3] = ROTL_md(wd_sse[3], 9);
		wd_sse[2] += (wd_sse[1] ^ wd_sse[0] ^ wd_sse[3]) + nt_buffer_sse[6]  +  SQRT_3_sse; wd_sse[2] = ROTL_md(wd_sse[2], 11);
		wd_sse[1] += (wd_sse[0] ^ wd_sse[3] ^ wd_sse[2]) + nt_buffer_sse[14] +  SQRT_3_sse; wd_sse[1] = ROTL_md(wd_sse[1], 15);
	 
		wd_sse[0] += (wd_sse[3] ^ wd_sse[2] ^ wd_sse[1]) + nt_buffer_sse[1]  +  SQRT_3_sse; wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += (wd_sse[2] ^ wd_sse[1] ^ wd_sse[0]) + nt_buffer_sse[9]  +  SQRT_3_sse; wd_sse[3] = ROTL_md(wd_sse[3], 9);
		wd_sse[2] += (wd_sse[1] ^ wd_sse[0] ^ wd_sse[3]) + nt_buffer_sse[5]  +  SQRT_3_sse; wd_sse[2] = ROTL_md(wd_sse[2], 11);
		wd_sse[1] += (wd_sse[0] ^ wd_sse[3] ^ wd_sse[2]) + nt_buffer_sse[13] +  SQRT_3_sse; wd_sse[1] = ROTL_md(wd_sse[1], 15);
	 
		wd_sse[0] += (wd_sse[3] ^ wd_sse[2] ^ wd_sse[1]) + nt_buffer_sse[3]  +  SQRT_3_sse; wd_sse[0] = ROTL_md(wd_sse[0], 3);
		wd_sse[3] += (wd_sse[2] ^ wd_sse[1] ^ wd_sse[0]) + nt_buffer_sse[11] +  SQRT_3_sse; wd_sse[3] = ROTL_md(wd_sse[3], 9);
		wd_sse[2] += (wd_sse[1] ^ wd_sse[0] ^ wd_sse[3]) + nt_buffer_sse[7]  +  SQRT_3_sse; wd_sse[2] = ROTL_md(wd_sse[2], 11);
		wd_sse[1] += (wd_sse[0] ^ wd_sse[3] ^ wd_sse[2]) + nt_buffer_sse[15] +  SQRT_3_sse; wd_sse[1] = ROTL_md(wd_sse[1], 15);
	}

	inline void finalize_md4_md()
	{
		for(int i = 0; i  <4; i++)
			memcpy(wd_md[i], &wd_sse[i], (sizeof(unsigned int) * 4));

		for(int i = 0; i < 4; i++)
		{
			wd_md[0][i] += 0x67452301;
			wd_md[1][i] += 0xefcdab89;
			wd_md[2][i] += 0x98badcfe;
			wd_md[3][i] += 0x10325476;
		}
	}

	void convert_to_hex_md(std::string output[4])
	{
		for(int i = 0; i < 4; i++)
		{
			//Iterate the integer
			for(int j = 0;j < 4; j++)
			{
				unsigned int n = wd_md[j][i];
				//iterate the bytes of the integer		
				for(int k = 0; k < 4; k++)
				{
					unsigned int convert = n % 256;
					hex_format[j*8+k*2+1]=itoa16[convert%16];
					convert /= 16;
					hex_format[j*8+k*2+0]=itoa16[convert%16];
					n /= 256;
				}	
			}

			//null terminate the string
			hex_format[32]=0;

			output[i] = hex_format;
		}
	}

	int64_pair* convert_to_int128_md()
	{
		for(int i = 0; i < 4; i++)
		{
			int128_retval[i].first = (wd_md[i][0] << 31) | wd_md[i][1];
			int128_retval[i].second = (wd_md[i][2] << 31) | wd_md[i][3];
		}

		return int128_retval;
	}

	// The 'md' suffix stands for "multiple data"
	__m128i nt_buffer_sse[16];
	unsigned int nt_buffer_md[16][4];

	__m128i wd_sse[4];
	unsigned int wd_md[4][4];

	__m128i SQRT_2_sse, SQRT_3_sse;

	int64_pair int128_retval[4];
};

#endif