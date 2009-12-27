#ifndef NTLM_MD_
#define NTLM_MD_

#include "NTLM.h"

#include <emmintrin.h>

// Overloaded operators for dealing with the 128-bit sse datatype
__m128i operator +(const __m128i &a, const __m128i &b)
{
	return _mm_add_epi32(a, b);
}

__m128i operator +(const __m128i &a, const unsigned int b_sd[4])
{
	__m128i b = _mm_set_epi32(b_sd[3], b_sd[2], b_sd[1], b_sd[0]);
	return _mm_add_epi32(a, b);
}

__m128i operator +(const __m128i &a, const unsigned int b_scalar)
{
	__m128i b = _mm_set1_epi32(b_scalar);
	return _mm_add_epi32(a, b);
}

__m128i operator +=(__m128i &a, const __m128i &b)
{
	return a = a + b;
}

__m128i operator +=(__m128i &a, const int b_scalar)
{
	return a = a + b_scalar;
}

__m128i operator -(const __m128i &a, const __m128i &b)
{
	return _mm_sub_epi32(a, b);
}

__m128i operator ^(const __m128i &a, const __m128i &b)
{
	return _mm_xor_si128(a, b);
}

__m128i operator |(const __m128i &a, const __m128i &b)
{
	return _mm_or_si128(a, b);
}

__m128i operator &(const __m128i &a, const __m128i &b)
{
	return _mm_and_si128(a, b);
}

// SSE rotate function
__m128i ROTL_md(const __m128i &input, const int places)
{
	return _mm_or_si128(_mm_slli_epi32(input, places), _mm_srli_epi32(input, (32 - places)));
}

class NTLM_SSE2 : public NTLM
{
public:

	NTLM_SSE2()
	{
		SQRT_2_md = _mm_set1_epi32(SQRT_2);
		SQRT_3_md = _mm_set1_epi32(SQRT_3);
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
		wd0 = _mm_set1_epi32(0x67452301);
		wd1 = _mm_set1_epi32(0xefcdab89);
		wd2 = _mm_set1_epi32(0x98badcfe);
		wd3 = _mm_set1_epi32(0x10325476);
	}

	void md4_crypt_round1_md()
	{	 
		wd0 += (wd3 ^ (wd1 & (wd2 ^ wd3)))  +  nt_buffer_md_sse[0];  wd0 = ROTL_md(wd0, 3);
		wd3 += (wd2 ^ (wd0 & (wd1 ^ wd2)))  +  nt_buffer_md_sse[1];  wd3 = ROTL_md(wd3, 7);
		wd2 += (wd1 ^ (wd3 & (wd0 ^ wd1)))  +  nt_buffer_md_sse[2];  wd2 = ROTL_md(wd2, 11);
		wd1 += (wd0 ^ (wd2 & (wd3 ^ wd0)))  +  nt_buffer_md_sse[3];  wd1 = ROTL_md(wd1, 19);
	 
		wd0 += (wd3 ^ (wd1 & (wd2 ^ wd3)))  +  nt_buffer_md_sse[4];  wd0 = ROTL_md(wd0, 3);
		wd3 += (wd2 ^ (wd0 & (wd1 ^ wd2)))  +  nt_buffer_md_sse[5];  wd3 = ROTL_md(wd3, 7);
		wd2 += (wd1 ^ (wd3 & (wd0 ^ wd1)))  +  nt_buffer_md_sse[6];  wd2 = ROTL_md(wd2, 11);
		wd1 += (wd0 ^ (wd2 & (wd3 ^ wd0)))  +  nt_buffer_md_sse[7];  wd1 = ROTL_md(wd1, 19);
	 
		wd0 += (wd3 ^ (wd1 & (wd2 ^ wd3)))  +  nt_buffer_md_sse[8];  wd0 = ROTL_md(wd0, 3);
		wd3 += (wd2 ^ (wd0 & (wd1 ^ wd2)))  +  nt_buffer_md_sse[9];  wd3 = ROTL_md(wd3, 7);
		wd2 += (wd1 ^ (wd3 & (wd0 ^ wd1)))  +  nt_buffer_md_sse[10]; wd2 = ROTL_md(wd2, 11);
		wd1 += (wd0 ^ (wd2 & (wd3 ^ wd0)))  +  nt_buffer_md_sse[11]; wd1 = ROTL_md(wd1, 19);
	 
		wd0 += (wd3 ^ (wd1 & (wd2 ^ wd3)))  +  nt_buffer_md_sse[12]; wd0 = ROTL_md(wd0, 3);
		wd3 += (wd2 ^ (wd0 & (wd1 ^ wd2)))  +  nt_buffer_md_sse[13]; wd3 = ROTL_md(wd3, 7);
		wd2 += (wd1 ^ (wd3 & (wd0 ^ wd1)))  +  nt_buffer_md_sse[14]; wd2 = ROTL_md(wd2, 11);
		wd1 += (wd0 ^ (wd2 & (wd3 ^ wd0)))  +  nt_buffer_md_sse[15]; wd1 = ROTL_md(wd1, 19);
	}
	 
	void md4_crypt_round2_md()
	{
		wd0 += ((wd1 & (wd2 | wd3)) | (wd2 & wd3)) + nt_buffer_md_sse[0] +SQRT_2_md; wd0 = ROTL_md(wd0, 3);
		wd3 += ((wd0 & (wd1 | wd2)) | (wd1 & wd2)) + nt_buffer_md_sse[4] +SQRT_2_md; wd3 = ROTL_md(wd3, 5);
		wd2 += ((wd3 & (wd0 | wd1)) | (wd0 & wd1)) + nt_buffer_md_sse[8] +SQRT_2_md; wd2 = ROTL_md(wd2, 9);
		wd1 += ((wd2 & (wd3 | wd0)) | (wd3 & wd0)) + nt_buffer_md_sse[12]+SQRT_2_md; wd1 = ROTL_md(wd1, 13);
	 
		wd0 += ((wd1 & (wd2 | wd3)) | (wd2 & wd3)) + nt_buffer_md_sse[1] +SQRT_2_md; wd0 = ROTL_md(wd0, 3);
		wd3 += ((wd0 & (wd1 | wd2)) | (wd1 & wd2)) + nt_buffer_md_sse[5] +SQRT_2_md; wd3 = ROTL_md(wd3, 5);
		wd2 += ((wd3 & (wd0 | wd1)) | (wd0 & wd1)) + nt_buffer_md_sse[9] +SQRT_2_md; wd2 = ROTL_md(wd2, 9);
		wd1 += ((wd2 & (wd3 | wd0)) | (wd3 & wd0)) + nt_buffer_md_sse[13]+SQRT_2_md; wd1 = ROTL_md(wd1, 13);
	 
		wd0 += ((wd1 & (wd2 | wd3)) | (wd2 & wd3)) + nt_buffer_md_sse[2] +SQRT_2_md; wd0 = ROTL_md(wd0, 3);
		wd3 += ((wd0 & (wd1 | wd2)) | (wd1 & wd2)) + nt_buffer_md_sse[6] +SQRT_2_md; wd3 = ROTL_md(wd3, 5);
		wd2 += ((wd3 & (wd0 | wd1)) | (wd0 & wd1)) + nt_buffer_md_sse[10]+SQRT_2_md; wd2 = ROTL_md(wd2, 9);
		wd1 += ((wd2 & (wd3 | wd0)) | (wd3 & wd0)) + nt_buffer_md_sse[14]+SQRT_2_md; wd1 = ROTL_md(wd1, 13);
	 
		wd0 += ((wd1 & (wd2 | wd3)) | (wd2 & wd3)) + nt_buffer_md_sse[3] +SQRT_2_md; wd0 = ROTL_md(wd0, 3);
		wd3 += ((wd0 & (wd1 | wd2)) | (wd1 & wd2)) + nt_buffer_md_sse[7] +SQRT_2_md; wd3 = ROTL_md(wd3, 5);
		wd2 += ((wd3 & (wd0 | wd1)) | (wd0 & wd1)) + nt_buffer_md_sse[11]+SQRT_2_md; wd2 = ROTL_md(wd2, 9);
		wd1 += ((wd2 & (wd3 | wd0)) | (wd3 & wd0)) + nt_buffer_md_sse[15]+SQRT_2_md; wd1 = ROTL_md(wd1, 13);
	}
	 
	void md4_crypt_round3_md()
	{
		wd0 += (wd3 ^ wd2 ^ wd1) + nt_buffer_md_sse[0]  +  SQRT_3_md; wd0 = ROTL_md(wd0, 3);
		wd3 += (wd2 ^ wd1 ^ wd0) + nt_buffer_md_sse[8]  +  SQRT_3_md; wd3 = ROTL_md(wd3, 9);
		wd2 += (wd1 ^ wd0 ^ wd3) + nt_buffer_md_sse[4]  +  SQRT_3_md; wd2 = ROTL_md(wd2, 11);
		wd1 += (wd0 ^ wd3 ^ wd2) + nt_buffer_md_sse[12] +  SQRT_3_md; wd1 = ROTL_md(wd1, 15);
	 
		wd0 += (wd3 ^ wd2 ^ wd1) + nt_buffer_md_sse[2]  +  SQRT_3_md; wd0 = ROTL_md(wd0, 3);
		wd3 += (wd2 ^ wd1 ^ wd0) + nt_buffer_md_sse[10] +  SQRT_3_md; wd3 = ROTL_md(wd3, 9);
		wd2 += (wd1 ^ wd0 ^ wd3) + nt_buffer_md_sse[6]  +  SQRT_3_md; wd2 = ROTL_md(wd2, 11);
		wd1 += (wd0 ^ wd3 ^ wd2) + nt_buffer_md_sse[14] +  SQRT_3_md; wd1 = ROTL_md(wd1, 15);
	 
		wd0 += (wd3 ^ wd2 ^ wd1) + nt_buffer_md_sse[1]  +  SQRT_3_md; wd0 = ROTL_md(wd0, 3);
		wd3 += (wd2 ^ wd1 ^ wd0) + nt_buffer_md_sse[9]  +  SQRT_3_md; wd3 = ROTL_md(wd3, 9);
		wd2 += (wd1 ^ wd0 ^ wd3) + nt_buffer_md_sse[5]  +  SQRT_3_md; wd2 = ROTL_md(wd2, 11);
		wd1 += (wd0 ^ wd3 ^ wd2) + nt_buffer_md_sse[13] +  SQRT_3_md; wd1 = ROTL_md(wd1, 15);
	 
		wd0 += (wd3 ^ wd2 ^ wd1) + nt_buffer_md_sse[3]  +  SQRT_3_md; wd0 = ROTL_md(wd0, 3);
		wd3 += (wd2 ^ wd1 ^ wd0) + nt_buffer_md_sse[11] +  SQRT_3_md; wd3 = ROTL_md(wd3, 9);
		wd2 += (wd1 ^ wd0 ^ wd3) + nt_buffer_md_sse[7]  +  SQRT_3_md; wd2 = ROTL_md(wd2, 11);
		wd1 += (wd0 ^ wd3 ^ wd2) + nt_buffer_md_sse[15] +  SQRT_3_md; wd1 = ROTL_md(wd1, 15);
	}

	inline void finalize_md4_md()
	{
		wd0 += 0x67452301;
		wd1 += 0xefcdab89;
		wd2 += 0x98badcfe;
		wd3 += 0x10325476;

		memcpy(wd_md[0], wd0.m128i_u32, (sizeof(unsigned int) * 4));
		memcpy(wd_md[1], wd1.m128i_u32, (sizeof(unsigned int) * 4));
		memcpy(wd_md[2], wd2.m128i_u32, (sizeof(unsigned int) * 4));
		memcpy(wd_md[3], wd3.m128i_u32, (sizeof(unsigned int) * 4));
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
	__m128i nt_buffer_md_sse[16];
	unsigned int nt_buffer_md[16][4];

	__m128i wd0, wd1, wd2, wd3, SQRT_2_md, SQRT_3_md;
	unsigned int wd_md[4][4];

	int64_pair int128_retval[4];
};

#endif