/* Slightly modified for LB, orginal code written by Alain Espinosa <alainesp@gmail.com>. Thanks, Alain! */

#ifndef MD4_H_
#define MD4_H_
 
#include <cstring>
#include <iostream>
#include <cstdlib>

#define ROTL(NUM, PLACES) ((NUM << PLACES) | (NUM >> (32 - PLACES)))
#define ROTR(NUM, PLACES) ((NUM >> PLACES) | (NUM << (32 - PLACES)))

#define F(X, Y, Z) ((X & Y) | (~X & Z))
#define G(X, Y, Z) ((X & Y) | (X & Z) | (Y & Z))
#define H(X, Y, Z) (X ^ Y ^ Z)

#define FF(A, B, C, D, X, ROT) \
	A += F(B, C, D) + X, A = ROTL(A, ROT)

#define GG(A, B, C, D, X, SQRT2, ROT) \
	A += G(B, C, D) + X + SQRT2, A = ROTL(A, ROT)

#define HH(A, B, C, D, X, SQRT3, ROT) \
	A += H(B, C, D) + X + SQRT3, A = ROTL(A, ROT)

class MD4
{
public:

	MD4()
	{
		itoa16 = (char*)"0123456789abcdef";

		SQRT_2 = 0x5a827999;
		SQRT_3 = 0x6ed9eba1;

		wd_init[0] = 0x67452301;
		wd_init[1] = 0xefcdab89;
		wd_init[2] = 0x98badcfe;
		wd_init[3] = 0x10325476;
	}

	~MD4()
	{
	}

	typedef char* (MD4::*scalarHashPtr)(const char*);

	// Full MD4
	inline char* getHash_MD4(const char* input)
	{
		// Take an ASCII string, and convert it to an MD4 message
		prepare_key_md4(input);

		initialize_words();
		md4_crypt();
		finalize_md4();

		return convert_to_hex();
	}

	// Weak MD4
	inline char* getWeakHash_MD4(const char* input)
	{
		prepare_key_md4(input);

		initialize_words();
		md4_crypt();

		return convert_to_int128();
	}

	// Full NTLM
	inline char* getHash_NTLM(const char* input)
	{
		prepare_key_ntlm(input);

		initialize_words();
		md4_crypt();
		finalize_md4();

		// Convert the result to hexadecimal
		return convert_to_hex();
	}

	// Weak NTLM
	char* getWeakHash_NTLM(const char* input)
	{
		prepare_key_ntlm(input);

		initialize_words();
		md4_crypt();

		return convert_to_int128();
	}

	// Take an MD4 hash as input, and reverse the hex encoding, returning a 128-bit integer
	inline char* weakenHash(const char* input)
	{
		convert_from_hex(input);
		return convert_to_int128();
	}

protected:

	inline void prepare_key_md4(const char* input)
	{
		int i=0;
		int length = (int)strlen(input);

		// Zero out the message buffer
		memset(md4_buffer,0,16*4);

		for(;i<length/4;i++)
			md4_buffer[i] = input[4*i] | (input[4*i+1]<<8) | (input[4*i+2]<<16) | (input[4*i+3]<<24);

		// Pad with one 1 bit, followed by zeros until the message is 64 bits shy of 512 bits in length.
		switch(length%4)
		{
		case 0:
			md4_buffer[i] = 0x80;
			break;
		case 1:
			md4_buffer[i] = input[length - 1] | 0x8000;
			break;
		case 2:
			md4_buffer[i] = input[length-2] | (input[length-1]<<8) | 0x800000;
			break;
		case 3:
			md4_buffer[i] = input[length-3] | (input[length-2]<<8) | (input[length-1]<<16) | 0x80000000;
			break;
		}

		// Add the length
		md4_buffer[14] = length << 3;
	}

	inline void prepare_key_ntlm(const char* input)
	{
		int i=0;
		int length=(int)(strlen(input));

		memset(md4_buffer,0,16*4);
		
		for(;i<length/2;i++)	
			md4_buffer[i] = input[2*i] | (input[2*i+1]<<16);
	 
		// Pad
		if(length%2==1)
			md4_buffer[i] = input[length-1] | 0x800000;
		else
			md4_buffer[i]=0x80;
		// Length
		md4_buffer[14] = length << 4;
	}

	inline void initialize_words()
	{
		wd[0] = wd_init[0];
		wd[1] = wd_init[1];
		wd[2] = wd_init[2];
		wd[3] = wd_init[3];
	}

	inline void md4_crypt()
	{	 
		// Round 1 // ---

		FF(wd[0], wd[1], wd[2], wd[3], md4_buffer[0], 3);
		FF(wd[3], wd[0], wd[1], wd[2], md4_buffer[1], 7);
		FF(wd[2], wd[3], wd[0], wd[1], md4_buffer[2], 11);
		FF(wd[1], wd[2], wd[3], wd[0], md4_buffer[3], 19);
	 
		FF(wd[0], wd[1], wd[2], wd[3], md4_buffer[4], 3);
		FF(wd[3], wd[0], wd[1], wd[2], md4_buffer[5], 7);
		FF(wd[2], wd[3], wd[0], wd[1], md4_buffer[6], 11);
		FF(wd[1], wd[2], wd[3], wd[0], md4_buffer[7], 19);
	 
		FF(wd[0], wd[1], wd[2], wd[3], md4_buffer[8],  3);
		FF(wd[3], wd[0], wd[1], wd[2], md4_buffer[9],  7);
		FF(wd[2], wd[3], wd[0], wd[1], md4_buffer[10], 11);
		FF(wd[1], wd[2], wd[3], wd[0], md4_buffer[11], 19);
	 
		FF(wd[0], wd[1], wd[2], wd[3], md4_buffer[12], 3);
		FF(wd[3], wd[0], wd[1], wd[2], md4_buffer[13], 7);
		FF(wd[2], wd[3], wd[0], wd[1], md4_buffer[14], 11);
		FF(wd[1], wd[2], wd[3], wd[0], md4_buffer[15], 19);

		// Round 2 // ---

		GG(wd[0], wd[1], wd[2], wd[3], md4_buffer[0],  SQRT_2, 3);
		GG(wd[3], wd[0], wd[1], wd[2], md4_buffer[4],  SQRT_2, 5);
		GG(wd[2], wd[3], wd[0], wd[1], md4_buffer[8],  SQRT_2, 9);
		GG(wd[1], wd[2], wd[3], wd[0], md4_buffer[12], SQRT_2, 13);
	 
		GG(wd[0], wd[1], wd[2], wd[3], md4_buffer[1],  SQRT_2, 3);
		GG(wd[3], wd[0], wd[1], wd[2], md4_buffer[5],  SQRT_2, 5);
		GG(wd[2], wd[3], wd[0], wd[1], md4_buffer[9],  SQRT_2, 9);
		GG(wd[1], wd[2], wd[3], wd[0], md4_buffer[13], SQRT_2, 13);
	 
		GG(wd[0], wd[1], wd[2], wd[3], md4_buffer[2],  SQRT_2, 3);
		GG(wd[3], wd[0], wd[1], wd[2], md4_buffer[6],  SQRT_2, 5);
		GG(wd[2], wd[3], wd[0], wd[1], md4_buffer[10], SQRT_2, 9);
		GG(wd[1], wd[2], wd[3], wd[0], md4_buffer[14], SQRT_2, 13);
	 
		GG(wd[0], wd[1], wd[2], wd[3], md4_buffer[3],  SQRT_2, 3);
		GG(wd[3], wd[0], wd[1], wd[2], md4_buffer[7],  SQRT_2, 5);
		GG(wd[2], wd[3], wd[0], wd[1], md4_buffer[11], SQRT_2, 9);
		GG(wd[1], wd[2], wd[3], wd[0], md4_buffer[15], SQRT_2, 13);
	
		// Round 3 // ---

		/* Since all elements past length/2 for NTLM and length/4 for MD4 are zero, 
		we can assume that a few of the elements in the message are zero. By taking a hash, and
		reversing it back to four 32-bit words, we can reverse certain sections of the hashing 
		process by substituting zero for the message section. Also, we might even be able to get
		a rough estimate of the length of the string that generated the hash by substituting zero
		for the message section, reversing, then redoing the line. If the same hash results, that
		element had to have been zero. We can do this until we find an element of the message that
		_wasn't_ zero, in which case we would have found the end of the message.*/

		HH(wd[0], wd[3], wd[2], wd[1], md4_buffer[0],  SQRT_3, 3);
		HH(wd[3], wd[2], wd[1], wd[0], md4_buffer[8],  SQRT_3, 9);
		HH(wd[2], wd[1], wd[0], wd[3], md4_buffer[4],  SQRT_3, 11);
		HH(wd[1], wd[0], wd[3], wd[2], md4_buffer[12], SQRT_3, 15);
	 
		HH(wd[0], wd[3], wd[2], wd[1], md4_buffer[2],  SQRT_3, 3);
		HH(wd[3], wd[2], wd[1], wd[0], md4_buffer[10], SQRT_3, 9);
		HH(wd[2], wd[1], wd[0], wd[3], md4_buffer[6],  SQRT_3, 11);
		HH(wd[1], wd[0], wd[3], wd[2], md4_buffer[14], SQRT_3, 15);
	 
		HH(wd[0], wd[3], wd[2], wd[1], md4_buffer[1],  SQRT_3, 3);
		HH(wd[3], wd[2], wd[1], wd[0], md4_buffer[9],  SQRT_3, 9);
		HH(wd[2], wd[1], wd[0], wd[3], md4_buffer[5],  SQRT_3, 11);
		HH(wd[1], wd[0], wd[3], wd[2], md4_buffer[13], SQRT_3, 15);
	 
		HH(wd[0], wd[3], wd[2], wd[1], md4_buffer[3],  SQRT_3, 3);
		HH(wd[3], wd[2], wd[1], wd[0], md4_buffer[11], SQRT_3, 9);
		HH(wd[2], wd[1], wd[0], wd[3], md4_buffer[7],  SQRT_3, 11);
		HH(wd[1], wd[0], wd[3], wd[2], md4_buffer[15], SQRT_3, 15);
	}
	 
	inline void finalize_md4()
	{
		wd[0] += wd_init[0];
		wd[1] += wd_init[1];
		wd[2] += wd_init[2];
		wd[3] += wd_init[3];
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

	// Convert a hex digest back to four 32-bit words
	void convert_from_hex(const char* hash)
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

		wd[0] -= wd_init[0];
		wd[1] -= wd_init[1];
		wd[2] -= wd_init[2];
		wd[3] -= wd_init[3];
	}

	// Convert the four 32-bit words used in the encryption process to a 128-bit integer
	inline char* convert_to_int128()
	{
		return (char*)wd;
	}

	unsigned int wd_init[4];

private:

	char* itoa16;

	char hex_format[33]; // Char array used to store the resulting hex digest

	unsigned int md4_buffer[16];

	unsigned int SQRT_2;
	unsigned int SQRT_3;

	unsigned int wd[4];
};

#endif

