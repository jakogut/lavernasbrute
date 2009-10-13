//Slightly modified for LB, orginal code written by Alain Espinosa <alainesp@gmail.com>.
//All the credit goes to him; this wouldn't have been possible without his work. (Crypto isn't my gig.)
//Thanks, Alain!

#ifndef NTLM_H_
#define NTLM_H_
 
#include <string.h>

class NTLM
{
public:

	NTLM()
	{
		itoa16 = "0123456789abcdef";
	}

	~NTLM()
	{
	}

	inline char* getNTLMHash(char* input)
	{
		prepare_key(input, nt_buffer);
		ntlm_crypt(nt_buffer, crypted);
		
		return convert_hex(crypted);
	}

protected:

	void prepare_key(char* input, unsigned long* output)
	{
		int i=0;
		int length=(int)(strlen(input));
		memset(output,0,16*4);
		//The length of input need to be <= 27
		for(;i<length/2;i++)	
			output[i] = input[2*i] | (input[2*i+1]<<16);
	 
		//padding
		if(length%2==1)
			output[i] = input[length-1] | 0x800000;
		else
			output[i]=0x80;
		//put the length
		output[14] = length << 4;
	}

	void ntlm_crypt(unsigned long* input, unsigned long* output)
	{
		unsigned long a = 0x67452301;
		unsigned long b = 0xefcdab89;
		unsigned long c = 0x98badcfe;
		unsigned long d = 0x10325476;

		unsigned long SQRT_2 = 0x5a827999;
		unsigned long SQRT_3 = 0x6ed9eba1;
	 
		a += (d ^ (b & (c ^ d)))  +  input[0]  ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  input[1]  ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  input[2]  ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  input[3]  ;b = (b << 19) | (b >> 13);
	 
		a += (d ^ (b & (c ^ d)))  +  input[4]  ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  input[5]  ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  input[6]  ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  input[7]  ;b = (b << 19) | (b >> 13);
	 
		a += (d ^ (b & (c ^ d)))  +  input[8]  ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  input[9]  ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  input[10] ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  input[11] ;b = (b << 19) | (b >> 13);
	 
		a += (d ^ (b & (c ^ d)))  +  input[12] ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  input[13] ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  input[14] ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  input[15] ;b = (b << 19) | (b >> 13);
	 

		a += ((b & (c | d)) | (c & d)) + input[0] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + input[4] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + input[8] +SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + input[12]+SQRT_2; b = (b<<13) | (b>>19);
	 
		a += ((b & (c | d)) | (c & d)) + input[1] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + input[5] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + input[9] +SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + input[13]+SQRT_2; b = (b<<13) | (b>>19);
	 
		a += ((b & (c | d)) | (c & d)) + input[2] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + input[6] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + input[10]+SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + input[14]+SQRT_2; b = (b<<13) | (b>>19);
	 
		a += ((b & (c | d)) | (c & d)) + input[3] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + input[7] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + input[11]+SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + input[15]+SQRT_2; b = (b<<13) | (b>>19);
	 

		a += (d ^ c ^ b) + input[0]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + input[8]  +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + input[4]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + input[12] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		a += (d ^ c ^ b) + input[2]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + input[10] +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + input[6]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + input[14] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		a += (d ^ c ^ b) + input[1]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + input[9]  +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + input[5]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + input[13] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		a += (d ^ c ^ b) + input[3]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + input[11] +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + input[7]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + input[15] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		output[0] = a + 0x67452301;
		output[1] = b + 0xefcdab89;
		output[2] = c + 0x98badcfe;
		output[3] = d + 0x10325476;
	}

	char* convert_hex(unsigned long* output)
	{
		//Iterate the integer
		for(int i = 0;i < 4; i++)
		{
			unsigned long n = output[i];
			//iterate the bytes of the integer		
			for(int j = 0; j < 4; j++)
			{
				unsigned long convert = n % 256;
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

	unsigned long nt_buffer[16];
	unsigned long crypted[4];
	char hex_format[33];
	char* itoa16;
};

#endif