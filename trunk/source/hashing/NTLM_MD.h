#ifndef NTLM_MD_
#define NTLM_MD_

#include "NTLM.h"
#include "OverloadSSE.h"

// The current implementation uses sucky, disgusting Visual C++ specific code.
#ifdef WIN32

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

	void getMultipleHashes(std::string* input, std::string* output)
	{
		prepare_key_md(input);

		initialize_words_md();

		md4_crypt_round1_md();
		md4_crypt_round2_md();
		md4_crypt_round3_md();

		finalize_md4_md();

		convert_to_hex_md(output);
	}

	void getMultipleWeakHashes(std::string* input, int64_pair* output)
	{
		prepare_key_md(input);

		initialize_words_md();

		md4_crypt_round1_md();
		md4_crypt_round2_md();
		md4_crypt_round3_md();

		finalize_md4_md();

		convert_to_int128_md(output);
	}

protected:

	// Rewrite this in SSE2
	void prepare_key_md(std::string input[4])
	{
		int length[4];

		for(int i = 0; i < 4; i++)
			length[i] = input[i].length();

		memset(nt_buffer_md_sse, 0, 16*4*4);

		for(int i = 0; i < 4; i++)
		{
			int j = 0;
			//The length of input need to be <= 27
			for(;j<length[i]/2;j++)	
				nt_buffer_md_sse[j].m128i_u32[i] = input[i].c_str()[2*j] | (input[i].c_str()[2*j+1]<<16);
		 
			//padding
			if(length[i]%2==1)
				nt_buffer_md_sse[j].m128i_u32[i] = input[i].c_str()[length[i]-1] | 0x800000;
			else
				nt_buffer_md_sse[j].m128i_u32[i]=0x80;
			//put the length
			nt_buffer_md_sse[14].m128i_u32[i] = length[i] << 4;
		}
	}

	void initialize_words_md()
	{
		wd[0] = _mm_set1_epi32(0x67452301);
		wd[1] = _mm_set1_epi32(0xefcdab89);
		wd[2] = _mm_set1_epi32(0x98badcfe);
		wd[3] = _mm_set1_epi32(0x10325476);
	}

	void md4_crypt_round1_md()
	{	 
		wd[0] += (wd[3] ^ (wd[1] & (wd[2] ^ wd[3])))  +  nt_buffer_md_sse[0];  wd[0] = ROTL_md(wd[0], 3);
		wd[3] += (wd[2] ^ (wd[0] & (wd[1] ^ wd[2])))  +  nt_buffer_md_sse[1];  wd[3] = ROTL_md(wd[3], 7);
		wd[2] += (wd[1] ^ (wd[3] & (wd[0] ^ wd[1])))  +  nt_buffer_md_sse[2];  wd[2] = ROTL_md(wd[2], 11);
		wd[1] += (wd[0] ^ (wd[2] & (wd[3] ^ wd[0])))  +  nt_buffer_md_sse[3];  wd[1] = ROTL_md(wd[1], 19);
	 
		wd[0] += (wd[3] ^ (wd[1] & (wd[2] ^ wd[3])))  +  nt_buffer_md_sse[4];  wd[0] = ROTL_md(wd[0], 3);
		wd[3] += (wd[2] ^ (wd[0] & (wd[1] ^ wd[2])))  +  nt_buffer_md_sse[5];  wd[3] = ROTL_md(wd[3], 7);
		wd[2] += (wd[1] ^ (wd[3] & (wd[0] ^ wd[1])))  +  nt_buffer_md_sse[6];  wd[2] = ROTL_md(wd[2], 11);
		wd[1] += (wd[0] ^ (wd[2] & (wd[3] ^ wd[0])))  +  nt_buffer_md_sse[7];  wd[1] = ROTL_md(wd[1], 19);
	 
		wd[0] += (wd[3] ^ (wd[1] & (wd[2] ^ wd[3])))  +  nt_buffer_md_sse[8];  wd[0] = ROTL_md(wd[0], 3);
		wd[3] += (wd[2] ^ (wd[0] & (wd[1] ^ wd[2])))  +  nt_buffer_md_sse[9];  wd[3] = ROTL_md(wd[3], 7);
		wd[2] += (wd[1] ^ (wd[3] & (wd[0] ^ wd[1])))  +  nt_buffer_md_sse[10]; wd[2] = ROTL_md(wd[2], 11);
		wd[1] += (wd[0] ^ (wd[2] & (wd[3] ^ wd[0])))  +  nt_buffer_md_sse[11]; wd[1] = ROTL_md(wd[1], 19);
	 
		wd[0] += (wd[3] ^ (wd[1] & (wd[2] ^ wd[3])))  +  nt_buffer_md_sse[12]; wd[0] = ROTL_md(wd[0], 3);
		wd[3] += (wd[2] ^ (wd[0] & (wd[1] ^ wd[2])))  +  nt_buffer_md_sse[13]; wd[3] = ROTL_md(wd[3], 7);
		wd[2] += (wd[1] ^ (wd[3] & (wd[0] ^ wd[1])))  +  nt_buffer_md_sse[14]; wd[2] = ROTL_md(wd[2], 11);
		wd[1] += (wd[0] ^ (wd[2] & (wd[3] ^ wd[0])))  +  nt_buffer_md_sse[15]; wd[1] = ROTL_md(wd[1], 19);
	}
	 
	void md4_crypt_round2_md()
	{
		wd[0] += ((wd[1] & (wd[2] | wd[3])) | (wd[2] & wd[3])) + nt_buffer_md_sse[0] +SQRT_2_md; wd[0] = ROTL_md(wd[0], 3);
		wd[3] += ((wd[0] & (wd[1] | wd[2])) | (wd[1] & wd[2])) + nt_buffer_md_sse[4] +SQRT_2_md; wd[3] = ROTL_md(wd[3], 5);
		wd[2] += ((wd[3] & (wd[0] | wd[1])) | (wd[0] & wd[1])) + nt_buffer_md_sse[8] +SQRT_2_md; wd[2] = ROTL_md(wd[2], 9);
		wd[1] += ((wd[2] & (wd[3] | wd[0])) | (wd[3] & wd[0])) + nt_buffer_md_sse[12]+SQRT_2_md; wd[1] = ROTL_md(wd[1], 13);
	 
		wd[0] += ((wd[1] & (wd[2] | wd[3])) | (wd[2] & wd[3])) + nt_buffer_md_sse[1] +SQRT_2_md; wd[0] = ROTL_md(wd[0], 3);
		wd[3] += ((wd[0] & (wd[1] | wd[2])) | (wd[1] & wd[2])) + nt_buffer_md_sse[5] +SQRT_2_md; wd[3] = ROTL_md(wd[3], 5);
		wd[2] += ((wd[3] & (wd[0] | wd[1])) | (wd[0] & wd[1])) + nt_buffer_md_sse[9] +SQRT_2_md; wd[2] = ROTL_md(wd[2], 9);
		wd[1] += ((wd[2] & (wd[3] | wd[0])) | (wd[3] & wd[0])) + nt_buffer_md_sse[13]+SQRT_2_md; wd[1] = ROTL_md(wd[1], 13);
	 
		wd[0] += ((wd[1] & (wd[2] | wd[3])) | (wd[2] & wd[3])) + nt_buffer_md_sse[2] +SQRT_2_md; wd[0] = ROTL_md(wd[0], 3);
		wd[3] += ((wd[0] & (wd[1] | wd[2])) | (wd[1] & wd[2])) + nt_buffer_md_sse[6] +SQRT_2_md; wd[3] = ROTL_md(wd[3], 5);
		wd[2] += ((wd[3] & (wd[0] | wd[1])) | (wd[0] & wd[1])) + nt_buffer_md_sse[10]+SQRT_2_md; wd[2] = ROTL_md(wd[2], 9);
		wd[1] += ((wd[2] & (wd[3] | wd[0])) | (wd[3] & wd[0])) + nt_buffer_md_sse[14]+SQRT_2_md; wd[1] = ROTL_md(wd[1], 13);
	 
		wd[0] += ((wd[1] & (wd[2] | wd[3])) | (wd[2] & wd[3])) + nt_buffer_md_sse[3] +SQRT_2_md; wd[0] = ROTL_md(wd[0], 3);
		wd[3] += ((wd[0] & (wd[1] | wd[2])) | (wd[1] & wd[2])) + nt_buffer_md_sse[7] +SQRT_2_md; wd[3] = ROTL_md(wd[3], 5);
		wd[2] += ((wd[3] & (wd[0] | wd[1])) | (wd[0] & wd[1])) + nt_buffer_md_sse[11]+SQRT_2_md; wd[2] = ROTL_md(wd[2], 9);
		wd[1] += ((wd[2] & (wd[3] | wd[0])) | (wd[3] & wd[0])) + nt_buffer_md_sse[15]+SQRT_2_md; wd[1] = ROTL_md(wd[1], 13);
	}
	 
	void md4_crypt_round3_md()
	{
		wd[0] += (wd[3] ^ wd[2] ^ wd[1]) + nt_buffer_md_sse[0]  +  SQRT_3_md; wd[0] = ROTL_md(wd[0], 3);
		wd[3] += (wd[2] ^ wd[1] ^ wd[0]) + nt_buffer_md_sse[8]  +  SQRT_3_md; wd[3] = ROTL_md(wd[3], 9);
		wd[2] += (wd[1] ^ wd[0] ^ wd[3]) + nt_buffer_md_sse[4]  +  SQRT_3_md; wd[2] = ROTL_md(wd[2], 11);
		wd[1] += (wd[0] ^ wd[3] ^ wd[2]) + nt_buffer_md_sse[12] +  SQRT_3_md; wd[1] = ROTL_md(wd[1], 15);
	 
		wd[0] += (wd[3] ^ wd[2] ^ wd[1]) + nt_buffer_md_sse[2]  +  SQRT_3_md; wd[0] = ROTL_md(wd[0], 3);
		wd[3] += (wd[2] ^ wd[1] ^ wd[0]) + nt_buffer_md_sse[10] +  SQRT_3_md; wd[3] = ROTL_md(wd[3], 9);
		wd[2] += (wd[1] ^ wd[0] ^ wd[3]) + nt_buffer_md_sse[6]  +  SQRT_3_md; wd[2] = ROTL_md(wd[2], 11);
		wd[1] += (wd[0] ^ wd[3] ^ wd[2]) + nt_buffer_md_sse[14] +  SQRT_3_md; wd[1] = ROTL_md(wd[1], 15);
	 
		wd[0] += (wd[3] ^ wd[2] ^ wd[1]) + nt_buffer_md_sse[1]  +  SQRT_3_md; wd[0] = ROTL_md(wd[0], 3);
		wd[3] += (wd[2] ^ wd[1] ^ wd[0]) + nt_buffer_md_sse[9]  +  SQRT_3_md; wd[3] = ROTL_md(wd[3], 9);
		wd[2] += (wd[1] ^ wd[0] ^ wd[3]) + nt_buffer_md_sse[5]  +  SQRT_3_md; wd[2] = ROTL_md(wd[2], 11);
		wd[1] += (wd[0] ^ wd[3] ^ wd[2]) + nt_buffer_md_sse[13] +  SQRT_3_md; wd[1] = ROTL_md(wd[1], 15);
	 
		wd[0] += (wd[3] ^ wd[2] ^ wd[1]) + nt_buffer_md_sse[3]  +  SQRT_3_md; wd[0] = ROTL_md(wd[0], 3);
		wd[3] += (wd[2] ^ wd[1] ^ wd[0]) + nt_buffer_md_sse[11] +  SQRT_3_md; wd[3] = ROTL_md(wd[3], 9);
		wd[2] += (wd[1] ^ wd[0] ^ wd[3]) + nt_buffer_md_sse[7]  +  SQRT_3_md; wd[2] = ROTL_md(wd[2], 11);
		wd[1] += (wd[0] ^ wd[3] ^ wd[2]) + nt_buffer_md_sse[15] +  SQRT_3_md; wd[1] = ROTL_md(wd[1], 15);
	}

	inline void finalize_md4_md()
	{
		//memcpy(wd_md, wd, (sizeof(unsigned int) * 4 * 4));

		for(int i = 0; i < 4; i++)
			_mm_store_si128((__m128i*)wd_md[i], wd[i]);

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
			for(int j = 0; j < 4; j++)
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

	void convert_to_int128_md(int64_pair* output)
	{
		for(int i = 0; i < 4; i++)
		{
			output[i].first = (wd_md[0][i] << 31) | wd_md[1][i];
			output[i].second = (wd_md[0][i] << 31) | wd_md[3][i];
		}
	}

	// The 'md' suffix stands for "multiple data"
	__m128i nt_buffer_md_sse[16];
	unsigned int nt_buffer_md[16][4];

	__m128i wd[4];
	unsigned int wd_md[4][4];

	__m128i SQRT_2_md, SQRT_3_md;
};

#endif

#endif