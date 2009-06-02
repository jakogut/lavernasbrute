//Slightly modified for LB, orginal code written by Alain Espinosa <alainesp@gmail.com>.
//All the credit goes to him; this wouldn't have been possible without his work. (Crypto isn't my gig.)
//Thanks, Alain!

#ifndef NTLM_H_
#define NTLM_H_
 
#include <string.h>

#include "StreamOperations.h"
 
using namespace std;

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

	inline static string getNTLMHash(string input)
	{
		prepare_key((char*)input.c_str(), nt_buffer);
		ntlm_crypt(nt_buffer, crypted);
		
		return convert_hex(crypted);
	}

protected:

	static void prepare_key(char *key, unsigned int nt_buffer[])
	{
		int i=0;
		int length=(int)(strlen(key));
		memset(nt_buffer,0,16*4);
		//The length of key need to be <= 27
		for(;i<length/2;i++)	
			nt_buffer[i] = key[2*i] | (key[2*i+1]<<16);
	 
		//padding
		if(length%2==1)
			nt_buffer[i] = key[length-1] | 0x800000;
		else
			nt_buffer[i]=0x80;
		//put the length
		nt_buffer[14] = length << 4;
	}

	static void ntlm_crypt(unsigned int nt_buffer[], unsigned int output[])
	{
		unsigned int a = 0x67452301;
		unsigned int b = 0xefcdab89;
		unsigned int c = 0x98badcfe;
		unsigned int d = 0x10325476;

		unsigned int SQRT_2 = 0x5a827999;
		unsigned int SQRT_3 = 0x6ed9eba1;
	 
		a += (d ^ (b & (c ^ d)))  +  nt_buffer[0]  ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  nt_buffer[1]  ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  nt_buffer[2]  ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  nt_buffer[3]  ;b = (b << 19) | (b >> 13);
	 
		a += (d ^ (b & (c ^ d)))  +  nt_buffer[4]  ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  nt_buffer[5]  ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  nt_buffer[6]  ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  nt_buffer[7]  ;b = (b << 19) | (b >> 13);
	 
		a += (d ^ (b & (c ^ d)))  +  nt_buffer[8]  ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  nt_buffer[9]  ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  nt_buffer[10] ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  nt_buffer[11] ;b = (b << 19) | (b >> 13);
	 
		a += (d ^ (b & (c ^ d)))  +  nt_buffer[12] ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  nt_buffer[13] ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  nt_buffer[14] ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  nt_buffer[15] ;b = (b << 19) | (b >> 13);
	 

		a += ((b & (c | d)) | (c & d)) + nt_buffer[0] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + nt_buffer[4] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + nt_buffer[8] +SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + nt_buffer[12]+SQRT_2; b = (b<<13) | (b>>19);
	 
		a += ((b & (c | d)) | (c & d)) + nt_buffer[1] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + nt_buffer[5] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + nt_buffer[9] +SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + nt_buffer[13]+SQRT_2; b = (b<<13) | (b>>19);
	 
		a += ((b & (c | d)) | (c & d)) + nt_buffer[2] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + nt_buffer[6] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + nt_buffer[10]+SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + nt_buffer[14]+SQRT_2; b = (b<<13) | (b>>19);
	 
		a += ((b & (c | d)) | (c & d)) + nt_buffer[3] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + nt_buffer[7] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + nt_buffer[11]+SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + nt_buffer[15]+SQRT_2; b = (b<<13) | (b>>19);
	 

		a += (d ^ c ^ b) + nt_buffer[0]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + nt_buffer[8]  +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + nt_buffer[4]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + nt_buffer[12] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		a += (d ^ c ^ b) + nt_buffer[2]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + nt_buffer[10] +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + nt_buffer[6]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + nt_buffer[14] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		a += (d ^ c ^ b) + nt_buffer[1]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + nt_buffer[9]  +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + nt_buffer[5]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + nt_buffer[13] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		a += (d ^ c ^ b) + nt_buffer[3]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + nt_buffer[11] +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + nt_buffer[7]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + nt_buffer[15] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		output[0] = a + (unsigned int)0x67452301;
		output[1] = b + (unsigned int)0xefcdab89;
		output[2] = c + (unsigned int)0x98badcfe;
		output[3] = d + (unsigned int)0x10325476;
	}

	static char* convert_hex(unsigned int output[])
	{
		//Iterate the integer
		for(int i = 0;i < 4; i++)
		{
			unsigned int n=output[i];
			//iterate the bytes of the integer		
			for(int j = 0; j < 4; j++)
			{
				unsigned int convert=n%256;
				hex_format[i*8+j*2+1]=itoa16[convert%16];
				convert/=16;
				hex_format[i*8+j*2+0]=itoa16[convert%16];
				n/=256;
			}	
		}
		//null terminate the string
		hex_format[33]=0;

		return hex_format;
	}

protected:

	static unsigned int nt_buffer[16];
	static unsigned int crypted[4];
	static char* itoa16;
	static char hex_format[33];
};

#endif