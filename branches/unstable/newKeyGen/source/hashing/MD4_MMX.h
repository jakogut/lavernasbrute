//Part of Laverna's Brute

//////////////////////////////////////////
////* --- THIS DOES NOT WORK YET --- *////
//////////////////////////////////////////

#include <xmmintrin.h>

#include "MD4.h"
#include "KeyGenerator.h"

#define ROTL_mmx(num, places) (_m_por(_m_pslld(num, _m_from_int(places)), _m_psrld(num, _m_from_int(32 - places))))
 
#define round1_mmx(wd_index, a, b, c, ntb_index, rotation) \
	wd[wd_index] = _m_paddd(wd[wd_index], _m_paddd(_m_por(_m_pand(wd[a], wd[b]), _m_pandn(wd[a], wd[c])), md4_buffer[ntb_index])), \
	wd[wd_index] = ROTL_mmx(wd[wd_index], rotation)

#define round2_mmx(wd_index, a, b, c, ntb_index, rotation) \
	wd[wd_index] = _m_paddd(wd[wd_index], _m_paddd(_m_paddd(_m_por(_m_pand(wd[a], _m_por(wd[b], wd[c])), _m_pand(wd[b], wd[c])), md4_buffer[ntb_index]), SQRT_2)), \
	wd[wd_index] = ROTL_mmx(wd[wd_index], rotation)

#define round3_mmx(wd_index, a, b, c, ntb_index, rotation) \
	wd[wd_index] = _m_paddd(_m_paddd(_m_paddd(wd[wd_index], _m_pxor(wd[a], _m_pxor(wd[b], wd[c]))), md4_buffer[ntb_index]), SQRT_3), \
	wd[wd_index] = ROTL_mmx(wd[wd_index], rotation)

class MD4_MMX
{
public:

	MD4_MMX()
	{
		SQRT_2 = _m_from_int(0x5a827999);
		SQRT_3 = _m_from_int(0x6ed9eba1);
	}

	~MD4_MMX()
	{
	}

	void getWeakHashes_NTLM(std::string* input, hashContext_MD4* output)
	{
		prepare_key_ntlm_mmx(input);

		initialize_words_mmx();

		md4_crypt_mmx();

		convert_to_int128_mmx(output);
	}

protected:

	inline void prepare_key_ntlm_mmx(std::string* input)
	{
		unsigned int length[2];

		for(int i = 0; i < 2; i++)
			length[i] = (unsigned int)input[i].length();

		memset(md4_buffer_md,0,16*4*2);

		for(int i = 0; i < 2; i++)
		{
			int j=0;

			//The length of input need to be <= 27
			for(;j<length[i]/2;j++)	
				md4_buffer_md[j][i] = input[i][2*j] | (input[i][2*j+1]<<16);
		 
			//padding
			if(length[i]%2==1)
				md4_buffer_md[j][i] = input[i][length[i]-1] | 0x800000;
			else
				md4_buffer_md[j][i]=0x80;
			//put the length
			md4_buffer_md[14][i] = length[i] << 4;
		}

		memcpy(md4_buffer, md4_buffer_md, 16*4*2);
	}

	inline void initialize_words_mmx()
	{
		wd[0] = _m_from_int(0x67452301);
		wd[1] = _m_from_int(0xefcdab89);
		wd[2] = _m_from_int(0x98badcfe);
		wd[3] = _m_from_int(0x10325476);

	}

	inline void md4_crypt_mmx()
	{
		// Round 1 // ---

		round1_mmx(0, 1, 2, 3, 0, 3);
		round1_mmx(3, 0, 1, 2, 1, 7);
		round1_mmx(2, 3, 0, 1, 2, 11);
		round1_mmx(1, 2, 3, 0, 3, 19);
		
		round1_mmx(0, 1, 2, 3, 4, 3);
		round1_mmx(3, 0, 1, 2, 5, 7);
		round1_mmx(2, 3, 0, 1, 6, 11);
		round1_mmx(1, 2, 3, 0, 7, 19);
		
		round1_mmx(0, 1, 2, 3, 8, 3);
		round1_mmx(3, 0, 1, 2, 9, 7);
		round1_mmx(2, 3, 0, 1, 10, 11);
		round1_mmx(1, 2, 3, 0, 11, 19);
		
		round1_mmx(0, 1, 2, 3, 12, 3);
		round1_mmx(3, 0, 1, 2, 13, 7);
		round1_mmx(2, 3, 0, 1, 14, 11);
		round1_mmx(1, 2, 3, 0, 15, 19);
		
		// Round 2 // ---

		round2_mmx(0, 1, 2, 3, 0, 3);
		round2_mmx(3, 0, 1, 2, 4, 5);
		round2_mmx(2, 3, 0, 1, 8, 9);
		round2_mmx(1, 2, 3, 0, 12, 13);
		
		round2_mmx(0, 1, 2, 3, 1, 3);
		round2_mmx(3, 0, 1, 2, 5, 5);
		round2_mmx(2, 3, 0, 1, 9, 9);
		round2_mmx(1, 2, 3, 0, 13, 13);
		
		round2_mmx(0, 1, 2, 3, 2, 3);
		round2_mmx(3, 0, 1, 2, 6, 5);
		round2_mmx(2, 3, 0, 1, 10, 9);
		round2_mmx(1, 2, 3, 0, 14, 13);

		round2_mmx(0, 1, 2, 3, 3, 3);
		round2_mmx(3, 0, 1, 2, 7, 5);
		round2_mmx(2, 3, 0, 1, 11, 9);
		round2_mmx(1, 2, 3, 0, 15, 13);
		
		// Round 3 // --

		round3_mmx(0, 3, 2, 1, 0, 3);
		round3_mmx(3, 2, 1, 0, 8, 9);
		round3_mmx(2, 1, 0, 3, 4, 11);
		round3_mmx(1, 0, 3, 2, 12, 15);
		
		round3_mmx(0, 3, 2, 1, 2, 3);
		round3_mmx(3, 2, 1, 0, 10, 9);
		round3_mmx(2, 1, 0, 3, 6, 11);
		round3_mmx(1, 0, 3, 2, 14, 15);
		
		round3_mmx(0, 3, 2, 1, 1, 3);
		round3_mmx(3, 2, 1, 0, 9, 9);
		round3_mmx(2, 1, 0, 3, 5, 11);
		round3_mmx(1, 0, 3, 2, 13, 15);
		
		round3_mmx(0, 3, 2, 1, 3, 3);
		round3_mmx(3, 2, 1, 0, 11, 9);
		round3_mmx(2, 1, 0, 3, 7, 11);
		round3_mmx(1, 0, 3, 2, 15, 15);

		memcpy(wd_md, wd, 4*4*2);

		_m_empty();
	}

	inline void convert_to_int128_mmx(hashContext_MD4* output)
	{
		for(int j = 0; j < 2; j++)
		{
			output[j].first  = ((unsigned long long)wd_md[0][j] << 32) | wd_md[2][j];
			output[j].second = ((unsigned long long)wd_md[1][j] << 32) | wd_md[3][j];
		}
	}

	__m64 md4_buffer[16];
	__m64 wd[4];

	__m64 SQRT_2, SQRT_3;

	// The 'md' suffix stands for "multiple data"
	unsigned int md4_buffer_md[16][2];

	unsigned int wd_md[4][2];

};