//Slightly modified for LB, orginal code written by Alain Espinosa <alainesp@gmail.com>.
//All the credit goes to him; this wouldn't have been possible without his work. (Crypto isn't my gig.)
//Thanks, Alain!

#ifndef NTLM_H_
#define NTLM_H_
 
#include <string.h>
#include <stdio.h>
#include <sstream>

#include <brook/brook.h>
#include <brook/stream.h>

#include "NTLM_GPU.h"
 
using namespace std;

class NTLM
{
public:

	NTLM()
	{
	}

	~NTLM()
	{
	}

	inline static string getNTLMHash(string input)
	{
		unsigned int nt_buffer[16];
		unsigned int crypted[4];
		unsigned char hexOutput[33];

		prepare_key((char*)input.c_str(), nt_buffer);
		ntlm_crypt(nt_buffer, crypted);
		
		return convert_hex(crypted);
	}

	static void getMultipleNTLMHashes(int size, string input[], string output[])
	{
		unsigned int streamSize[] = {size};

		unsigned int* cryptOutputArray_0 = new unsigned int[size];
		unsigned int* cryptOutputArray_1 = new unsigned int[size];
		unsigned int* cryptOutputArray_2 = new unsigned int[size];
		unsigned int* cryptOutputArray_3 = new unsigned int[size];

		unsigned int* cryptInputArray_0 = new unsigned int[size];
		unsigned int* cryptInputArray_1 = new unsigned int[size];
		unsigned int* cryptInputArray_2 = new unsigned int[size];
		unsigned int* cryptInputArray_3 = new unsigned int[size];
		unsigned int* cryptInputArray_4 = new unsigned int[size];
		unsigned int* cryptInputArray_5 = new unsigned int[size];
		unsigned int* cryptInputArray_6 = new unsigned int[size];
		unsigned int* cryptInputArray_7 = new unsigned int[size];
		unsigned int* cryptInputArray_8 = new unsigned int[size];
		unsigned int* cryptInputArray_9 = new unsigned int[size];
		unsigned int* cryptInputArray_10 = new unsigned int[size];
		unsigned int* cryptInputArray_11 = new unsigned int[size];
		unsigned int* cryptInputArray_12 = new unsigned int[size];
		unsigned int* cryptInputArray_13 = new unsigned int[size];
		unsigned int* cryptInputArray_14 = new unsigned int[size];
		unsigned int* cryptInputArray_15 = new unsigned int[size];

		unsigned int nt_buffer_single[16];
		unsigned int cryptInputArray[] = {size};

		unsigned int** cryptResult = new unsigned int*[size];

		//Create the second dimension
		for(int i = 0; i < size; ++i)
		{
			cryptResult[i] = new unsigned int[4];
		}

		for(int i = 0; i < size; ++i)
		{
			prepare_key((char*)input[i].c_str(), nt_buffer_single);

			cryptInputArray_0[i] = nt_buffer_single[0];
			cryptInputArray_1[i] = nt_buffer_single[1];
			cryptInputArray_2[i] = nt_buffer_single[2];
			cryptInputArray_3[i] = nt_buffer_single[3];
			cryptInputArray_4[i] = nt_buffer_single[4];
			cryptInputArray_5[i] = nt_buffer_single[5];
			cryptInputArray_6[i] = nt_buffer_single[6];
			cryptInputArray_7[i] = nt_buffer_single[7];
			cryptInputArray_8[i] = nt_buffer_single[8];
			cryptInputArray_9[i] = nt_buffer_single[9];
			cryptInputArray_10[i] = nt_buffer_single[10];
			cryptInputArray_11[i] = nt_buffer_single[11];
			cryptInputArray_12[i] = nt_buffer_single[12];
			cryptInputArray_13[i] = nt_buffer_single[13];
			cryptInputArray_14[i] = nt_buffer_single[14];
			cryptInputArray_15[i] = nt_buffer_single[15];
		}

		brook::Stream<uint> cryptInputStream_0(1, streamSize);
		brook::Stream<uint> cryptInputStream_1(1, streamSize);
		brook::Stream<uint> cryptInputStream_2(1, streamSize);
		brook::Stream<uint> cryptInputStream_3(1, streamSize);
		brook::Stream<uint> cryptInputStream_4(1, streamSize);
		brook::Stream<uint> cryptInputStream_5(1, streamSize);
		brook::Stream<uint> cryptInputStream_6(1, streamSize);
		brook::Stream<uint> cryptInputStream_7(1, streamSize);
		brook::Stream<uint> cryptInputStream_8(1, streamSize);
		brook::Stream<uint> cryptInputStream_9(1, streamSize);
		brook::Stream<uint> cryptInputStream_10(1, streamSize);
		brook::Stream<uint> cryptInputStream_11(1, streamSize);
		brook::Stream<uint> cryptInputStream_12(1, streamSize);
		brook::Stream<uint> cryptInputStream_13(1, streamSize);
		brook::Stream<uint> cryptInputStream_14(1, streamSize);
		brook::Stream<uint> cryptInputStream_15(1, streamSize);
		brook::Stream<uint> cryptInputStream_16(1, streamSize);

		brook::Stream<uint> cryptOutputStream_0(1, streamSize);
		brook::Stream<uint> cryptOutputStream_1(1, streamSize);
		brook::Stream<uint> cryptOutputStream_2(1, streamSize);
		brook::Stream<uint> cryptOutputStream_3(1, streamSize);

		streamRead(cryptInputStream_0, cryptInputArray_0);
		streamRead(cryptInputStream_1, cryptInputArray_1);
		streamRead(cryptInputStream_2, cryptInputArray_2);
		streamRead(cryptInputStream_3, cryptInputArray_3);
		streamRead(cryptInputStream_4, cryptInputArray_4);
		streamRead(cryptInputStream_5, cryptInputArray_5);
		streamRead(cryptInputStream_6, cryptInputArray_6);
		streamRead(cryptInputStream_7, cryptInputArray_7);
		streamRead(cryptInputStream_8, cryptInputArray_8);
		streamRead(cryptInputStream_9, cryptInputArray_9);
		streamRead(cryptInputStream_10, cryptInputArray_10);
		streamRead(cryptInputStream_11, cryptInputArray_11);
		streamRead(cryptInputStream_12, cryptInputArray_12);
		streamRead(cryptInputStream_13, cryptInputArray_13);
		streamRead(cryptInputStream_14, cryptInputArray_14);
		streamRead(cryptInputStream_15, cryptInputArray_15);

		gpu_ntlm_crypt(
			cryptInputStream_0, cryptInputStream_1, cryptInputStream_2, cryptInputStream_3,
			cryptInputStream_4, cryptInputStream_5, cryptInputStream_6, cryptInputStream_7,
			cryptInputStream_8, cryptInputStream_9, cryptInputStream_10, cryptInputStream_11,
			cryptInputStream_12, cryptInputStream_13, cryptInputStream_14, cryptInputStream_15, 
			cryptOutputStream_0, cryptOutputStream_1, cryptOutputStream_2, cryptOutputStream_3);

		streamWrite(cryptOutputStream_0, cryptOutputArray_0);
		streamWrite(cryptOutputStream_1, cryptOutputArray_1);
		streamWrite(cryptOutputStream_2, cryptOutputArray_2);
		streamWrite(cryptOutputStream_3, cryptOutputArray_3);

		//Recombine our split output
		for(int i = 0; i < size; ++i)
		{
			cryptResult[i][0] = cryptOutputArray_0[i];
			cryptResult[i][1] = cryptOutputArray_1[i];
			cryptResult[i][2] = cryptOutputArray_2[i];
			cryptResult[i][3] = cryptOutputArray_3[i];
		}

		for(int i = 0; i < size; ++i)
		{
			unsigned int toHex[4];

			for(int j = 0; j < 4; ++j)
			{
				toHex[j] = cryptResult[i][j];
			}

			output[i] = convert_hex(toHex);
		}

		delete [] cryptOutputArray_0;
		delete [] cryptOutputArray_1;
		delete [] cryptOutputArray_2;
		delete [] cryptOutputArray_3;

		delete [] cryptInputArray_0;
		delete [] cryptInputArray_1;
		delete [] cryptInputArray_2;
		delete [] cryptInputArray_3;
		delete [] cryptInputArray_4;
		delete [] cryptInputArray_5;
		delete [] cryptInputArray_6;
		delete [] cryptInputArray_7;
		delete [] cryptInputArray_8;
		delete [] cryptInputArray_9;
		delete [] cryptInputArray_10;
		delete [] cryptInputArray_11;
		delete [] cryptInputArray_12;
		delete [] cryptInputArray_13;
		delete [] cryptInputArray_14;
		delete [] cryptInputArray_15;

		for(int i = 0; i < size; ++i)
		{
			delete [] cryptResult[i];
		}

		delete [] cryptResult;
	}

protected:

	static void prepare_key(char *key, unsigned int output[])
	{
		int i=0;
		int length=strlen(key);
		memset(output,0,16*4);
		//The length of key need to be <= 27
		for(;i<length/2;i++)	
			output[i] = key[2*i] | (key[2*i+1]<<16);
	 
		//padding
		if(length%2==1)
			output[i] = key[length-1] | 0x800000;
		else
			output[i]=0x80;
		//put the length
		output[14] = length << 4;
	}

	static void ntlm_crypt(unsigned int nt_buffer[], unsigned int output[])
	{
		unsigned int a = 0x67452301;
		unsigned int b = 0xefcdab89;
		unsigned int c = 0x98badcfe;
		unsigned int d = 0x10325476;

		int SQRT_2(0x5a827999), SQRT_3(0x6ed9eba1);
	 
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
	 
		output[0] = a + 0x67452301;
		output[1] = b + 0xefcdab89;
		output[2] = c + 0x98badcfe;
		output[3] = d + 0x10325476;
	}

	static char* convert_hex(unsigned int intput[])
	{
		char hex_format[33];
		char itoa16[] = "0123456789abcdef";
		int i=0;
		//Iterate the integer
		for(;i<4;i++)
		{
			int j=0;
			unsigned int n=intput[i];
			//iterate the bytes of the integer		
			for(;j<4;j++)
			{
				unsigned int convert=n%256;
				hex_format[i*8+j*2+1]=itoa16[convert%16];
				convert=convert/16;
				hex_format[i*8+j*2+0]=itoa16[convert%16];
				n=n/256;
			}	
		}
		//null terminate the string
		hex_format[32]=0;

		return hex_format;
	}
};

#endif