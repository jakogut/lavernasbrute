/* Slightly modified for LB, orginal code written by Alain Espinosa <alainesp@gmail.com>.
All the credit goes to him; he made my life wonderfully easy by providing such a fine example of the MD4 algorithm.
Thanks, Alain! */

#ifndef NTLM_H_
#define NTLM_H_
 
#include <cstring>
#include <iostream>

#define ROTL(NUM, PLACES, SIZE) ((NUM << PLACES) | (NUM >> (SIZE - PLACES)))
#define ROTR(NUM, PLACES, SIZE) ((NUM >> PLACES) | (NUM << (SIZE - PLACES)))

typedef std::pair<unsigned long long, unsigned long long> int64_pair;

class NTLM
{
public:

	NTLM()
	{
		itoa16 = (char*)"0123456789abcdef";

		SQRT_2 = 0x5a827999;
		SQRT_3 = 0x6ed9eba1;
	}

	~NTLM()
	{
	}

	// Full NTLM hashing algorithm
	inline char* getNTLMHash(const std::string& input)
	{
		prepare_key((char*)input.c_str());

		// MD4 encrypt
		initialize_words();
		md4_crypt_round1();
		md4_crypt_round2();
		md4_crypt_round3();
		finalize_md4();
		
		// Convert the result to hexadecimal
		return convert_to_hex();
	}

	int64_pair getWeakHash(const std::string& input)
	{
		prepare_key((char*)input.c_str());

		initialize_words();
		md4_crypt_round1();
		md4_crypt_round2();
		md4_crypt_round3();

		return convert_to_int128();
	}

	// Take an NTLM hash as input, and reverse the hex encoding
	inline int64_pair weakenHash(const std::string& input)
	{
		convert_from_hex((char*)input.c_str());
		return convert_to_int128();
	}

protected:

	inline void prepare_key(const char* input)
	{
		int i=0;
		int length=(int)(strlen(input));
		memset(nt_buffer,0,16*4);
		//The length of input need to be <= 27
		for(;i<length/2;i++)	
			nt_buffer[i] = input[2*i] | (input[2*i+1]<<16);
	 
		//padding
		if(length%2==1)
			nt_buffer[i] = input[length-1] | 0x800000;
		else
			nt_buffer[i]=0x80;
		//put the length
		nt_buffer[14] = length << 4;
	}

	inline void initialize_words()
	{
		wd[0] = 0x67452301;
		wd[1] = 0xefcdab89;
		wd[2] = 0x98badcfe;
		wd[3] = 0x10325476;
	}

	inline void md4_crypt_round1()
	{	 
		wd[0] += (wd[3] ^ (wd[1] & (wd[2] ^ wd[3])))  +  nt_buffer[0];  wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += (wd[2] ^ (wd[0] & (wd[1] ^ wd[2])))  +  nt_buffer[1];  wd[3] = ROTL(wd[3], 7, 32);
		wd[2] += (wd[1] ^ (wd[3] & (wd[0] ^ wd[1])))  +  nt_buffer[2];  wd[2] = ROTL(wd[2], 11, 32);
		wd[1] += (wd[0] ^ (wd[2] & (wd[3] ^ wd[0])))  +  nt_buffer[3];  wd[1] = ROTL(wd[1], 19, 32);
	 
		wd[0] += (wd[3] ^ (wd[1] & (wd[2] ^ wd[3])))  +  nt_buffer[4];  wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += (wd[2] ^ (wd[0] & (wd[1] ^ wd[2])))  +  nt_buffer[5];  wd[3] = ROTL(wd[3], 7, 32);
		wd[2] += (wd[1] ^ (wd[3] & (wd[0] ^ wd[1])))  +  nt_buffer[6];  wd[2] = ROTL(wd[2], 11, 32);
		wd[1] += (wd[0] ^ (wd[2] & (wd[3] ^ wd[0])))  +  nt_buffer[7];  wd[1] = ROTL(wd[1], 19, 32);
	 
		wd[0] += (wd[3] ^ (wd[1] & (wd[2] ^ wd[3])))  +  nt_buffer[8];  wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += (wd[2] ^ (wd[0] & (wd[1] ^ wd[2])))  +  nt_buffer[9];  wd[3] = ROTL(wd[3], 7, 32);
		wd[2] += (wd[1] ^ (wd[3] & (wd[0] ^ wd[1])))  +  nt_buffer[10]; wd[2] = ROTL(wd[2], 11, 32);
		wd[1] += (wd[0] ^ (wd[2] & (wd[3] ^ wd[0])))  +  nt_buffer[11]; wd[1] = ROTL(wd[1], 19, 32);
	 
		wd[0] += (wd[3] ^ (wd[1] & (wd[2] ^ wd[3])))  +  nt_buffer[12]; wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += (wd[2] ^ (wd[0] & (wd[1] ^ wd[2])))  +  nt_buffer[13]; wd[3] = ROTL(wd[3], 7, 32);
		wd[2] += (wd[1] ^ (wd[3] & (wd[0] ^ wd[1])))  +  nt_buffer[14]; wd[2] = ROTL(wd[2], 11, 32);
		wd[1] += (wd[0] ^ (wd[2] & (wd[3] ^ wd[0])))  +  nt_buffer[15]; wd[1] = ROTL(wd[1], 19, 32);
	}
	 
	inline void md4_crypt_round2()
	{
		wd[0] += ((wd[1] & (wd[2] | wd[3])) | (wd[2] & wd[3])) + nt_buffer[0] +SQRT_2; wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += ((wd[0] & (wd[1] | wd[2])) | (wd[1] & wd[2])) + nt_buffer[4] +SQRT_2; wd[3] = ROTL(wd[3], 5, 32);
		wd[2] += ((wd[3] & (wd[0] | wd[1])) | (wd[0] & wd[1])) + nt_buffer[8] +SQRT_2; wd[2] = ROTL(wd[2], 9, 32);
		wd[1] += ((wd[2] & (wd[3] | wd[0])) | (wd[3] & wd[0])) + nt_buffer[12]+SQRT_2; wd[1] = ROTL(wd[1], 13, 32);
	 
		wd[0] += ((wd[1] & (wd[2] | wd[3])) | (wd[2] & wd[3])) + nt_buffer[1] +SQRT_2; wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += ((wd[0] & (wd[1] | wd[2])) | (wd[1] & wd[2])) + nt_buffer[5] +SQRT_2; wd[3] = ROTL(wd[3], 5, 32);
		wd[2] += ((wd[3] & (wd[0] | wd[1])) | (wd[0] & wd[1])) + nt_buffer[9] +SQRT_2; wd[2] = ROTL(wd[2], 9, 32);
		wd[1] += ((wd[2] & (wd[3] | wd[0])) | (wd[3] & wd[0])) + nt_buffer[13]+SQRT_2; wd[1] = ROTL(wd[1], 13, 32);
	 
		wd[0] += ((wd[1] & (wd[2] | wd[3])) | (wd[2] & wd[3])) + nt_buffer[2] +SQRT_2; wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += ((wd[0] & (wd[1] | wd[2])) | (wd[1] & wd[2])) + nt_buffer[6] +SQRT_2; wd[3] = ROTL(wd[3], 5, 32);
		wd[2] += ((wd[3] & (wd[0] | wd[1])) | (wd[0] & wd[1])) + nt_buffer[10]+SQRT_2; wd[2] = ROTL(wd[2], 9, 32);
		wd[1] += ((wd[2] & (wd[3] | wd[0])) | (wd[3] & wd[0])) + nt_buffer[14]+SQRT_2; wd[1] = ROTL(wd[1], 13, 32);
	 
		wd[0] += ((wd[1] & (wd[2] | wd[3])) | (wd[2] & wd[3])) + nt_buffer[3] +SQRT_2; wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += ((wd[0] & (wd[1] | wd[2])) | (wd[1] & wd[2])) + nt_buffer[7] +SQRT_2; wd[3] = ROTL(wd[3], 5, 32);
		wd[2] += ((wd[3] & (wd[0] | wd[1])) | (wd[0] & wd[1])) + nt_buffer[11]+SQRT_2; wd[2] = ROTL(wd[2], 9, 32);
		wd[1] += ((wd[2] & (wd[3] | wd[0])) | (wd[3] & wd[0])) + nt_buffer[15]+SQRT_2; wd[1] = ROTL(wd[1], 13, 32);
	}
	 
	inline void md4_crypt_round3()
	{
		wd[0] += (wd[3] ^ wd[2] ^ wd[1]) + nt_buffer[0]  +  SQRT_3; wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += (wd[2] ^ wd[1] ^ wd[0]) + nt_buffer[8]  +  SQRT_3; wd[3] = ROTL(wd[3], 9, 32);
		wd[2] += (wd[1] ^ wd[0] ^ wd[3]) + nt_buffer[4]  +  SQRT_3; wd[2] = ROTL(wd[2], 11, 32);
		wd[1] += (wd[0] ^ wd[3] ^ wd[2]) + nt_buffer[12] +  SQRT_3; wd[1] = ROTL(wd[1], 15, 32);
	 
		wd[0] += (wd[3] ^ wd[2] ^ wd[1]) + nt_buffer[2]  +  SQRT_3; wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += (wd[2] ^ wd[1] ^ wd[0]) + nt_buffer[10] +  SQRT_3; wd[3] = ROTL(wd[3], 9, 32);
		wd[2] += (wd[1] ^ wd[0] ^ wd[3]) + nt_buffer[6]  +  SQRT_3; wd[2] = ROTL(wd[2], 11, 32);
		wd[1] += (wd[0] ^ wd[3] ^ wd[2]) + nt_buffer[14] +  SQRT_3; wd[1] = ROTL(wd[1], 15, 32);
	 
		wd[0] += (wd[3] ^ wd[2] ^ wd[1]) + nt_buffer[1]  +  SQRT_3; wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += (wd[2] ^ wd[1] ^ wd[0]) + nt_buffer[9]  +  SQRT_3; wd[3] = ROTL(wd[3], 9, 32);
		wd[2] += (wd[1] ^ wd[0] ^ wd[3]) + nt_buffer[5]  +  SQRT_3; wd[2] = ROTL(wd[2], 11, 32);
		wd[1] += (wd[0] ^ wd[3] ^ wd[2]) + nt_buffer[13] +  SQRT_3; wd[1] = ROTL(wd[1], 15, 32);
	 
		wd[0] += (wd[3] ^ wd[2] ^ wd[1]) + nt_buffer[3]  +  SQRT_3; wd[0] = ROTL(wd[0], 3, 32);
		wd[3] += (wd[2] ^ wd[1] ^ wd[0]) + nt_buffer[11] +  SQRT_3; wd[3] = ROTL(wd[3], 9, 32);
		wd[2] += (wd[1] ^ wd[0] ^ wd[3]) + nt_buffer[7]  +  SQRT_3; wd[2] = ROTL(wd[2], 11, 32);
		wd[1] += (wd[0] ^ wd[3] ^ wd[2]) + nt_buffer[15] +  SQRT_3; wd[1] = ROTL(wd[1], 15, 32);
	}
	 
	inline void finalize_md4()
	{
		wd[0] += 0x67452301;
		wd[1] += 0xefcdab89;
		wd[2] += 0x98badcfe;
		wd[3] += 0x10325476;
	}

	char* convert_to_hex()
	{
		//Iterate the integer
		for(int i = 0;i < 4; i++)
		{
			unsigned int n = wd[i];
			//iterate the bytes of the integer		
			for(int j = 0; j < 4; j++)
			{
				unsigned int convert = n % 256;
				hex_format[i*8+j*2+1]=itoa16[convert%16];
				convert /= 16;
				hex_format[i*8+j*2+0]=itoa16[convert%16];
				n /= 256;
			}	
		}
		//null terminate the string
		hex_format[32]=0;

		return hex_format;
	}

	void convert_from_hex(char* hash)
	{
		char big_endian_hash[4][9];

		for(int i = 0; i < 4; i++)
		{
			// This needs to be reworked
			big_endian_hash[i][0] = hash[(i*8)+6];
			big_endian_hash[i][1] = hash[(i*8)+7];
			big_endian_hash[i][2] = hash[(i*8)+4];
			big_endian_hash[i][3] = hash[(i*8)+5];
			big_endian_hash[i][4] = hash[(i*8)+2];
			big_endian_hash[i][5] = hash[(i*8)+3];
			big_endian_hash[i][6] = hash[(i*8)+0];
			big_endian_hash[i][7] = hash[(i*8)+1];

			big_endian_hash[i][8] = '\0';
		}

		for(int i = 0; i < 4; i++)
		{
			wd[i] = strtoul(big_endian_hash[i], 0, 16);
		}

		wd[0] -= 0x67452301;
		wd[1] -= 0xefcdab89;
		wd[2] -= 0x98badcfe;
		wd[3] -= 0x10325476;
	}

	inline int64_pair convert_to_int128()
	{
		retval.first  = ((unsigned long long)wd[0] << 32) | wd[2];
		retval.second = ((unsigned long long)wd[1] << 32) | wd[3];

		return retval;
	}

	char hex_format[33];
	char* itoa16;

private:

	int64_pair retval;

	unsigned int SQRT_2;
	unsigned int SQRT_3;

	unsigned int nt_buffer[16];
	unsigned int wd[4];
};

#endif

