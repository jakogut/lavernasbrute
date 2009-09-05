//Slightly modified for LB, orginal code written by Alain Espinosa <alainesp@gmail.com>.
//All the credit goes to him; this wouldn't have been possible without his work. (Crypto isn't my gig.)
//Thanks, Alain!

#ifndef NTLM_H_
#define NTLM_H_

//GPU acceleration
#include "StreamOperations.h"

class NTLM
{
public:

	NTLM()
	{
		itoa16 = "0123456789abcdef";
	}

	NTLM(int size)
		: chunkSize(size)
	{
		itoa16 = "0123456789abcdef";

		gpu_nt_buffer = new unsigned int*[16];
		cpu_nt_buffer = new unsigned int*[size];
		gpu_crypted = new unsigned int*[4];
		cpu_crypted = new unsigned int*[size];

		for(int i = 0; i < 16; i++)
			gpu_nt_buffer[i] = new unsigned int[size];

		for(int i = 0; i < 4; i++)
			gpu_crypted[i] = new unsigned int[size];

		for(int i = 0; i < size; i++)
		{
			cpu_nt_buffer[i] = new unsigned int[16];
			cpu_crypted[i] = new unsigned int[4];
		}

		gpu = true;
	}

	~NTLM()
	{
		if(gpu)
		{
			delete [] *cpu_crypted;
			delete [] *gpu_crypted;
			delete [] *cpu_nt_buffer;
			delete [] *gpu_nt_buffer;

			delete [] cpu_crypted;
			delete [] gpu_crypted;
			delete [] cpu_nt_buffer;
			delete [] gpu_nt_buffer;
		}
	}

	char* getNTLMHash(std::string* input)
	{
		prepare_key((char*)input->c_str(), *nt_buffer);
		ntlm_crypt(*nt_buffer, crypted);
		
		return convert_hex(crypted);
	}

	char* getNTLMHash(std::string input)
	{
		prepare_key((char*)input.c_str(), *nt_buffer);
		ntlm_crypt(*nt_buffer, crypted);
		
		return convert_hex(crypted);
	}

	char* getNTLMHash(char* input)
	{
		prepare_key(input, *nt_buffer);
		ntlm_crypt(*nt_buffer, crypted);
		
		return convert_hex(crypted);
	}

	//FUGLY - Will be removed once the OpenCL path is finished.
	void getMultipleNTLMHashes(std::string* input, std::string* output)
	{
		if(gpu)
		{
			int size = chunkSize;

			for(int i = 0; i < size; i++)
			{
				prepare_key((char*)input[i].c_str(), cpu_nt_buffer[i]);

				for(int j = 0; j < 16; j++)
				{
					gpu_nt_buffer[j][i] = cpu_nt_buffer[i][j];
				}
			}

			gpu_ntlm_crypt(gpu_nt_buffer, gpu_crypted, size);
		
			for(int i = 0; i < size; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					cpu_nt_buffer[i][j] = gpu_nt_buffer[j][i];
					cpu_crypted[i][j] = gpu_crypted[j][i];
				}

				output[i] = convert_hex(cpu_crypted[i]);
			}
		}
		else
		{
			std::cerr << "ERROR: NTLM.h - Must specify array size!" << std::endl;
		}
	}

protected:

	void prepare_key(char* input, unsigned int* output)
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

	void ntlm_crypt(unsigned int* input, unsigned int* output)
	{
		unsigned int a = 0x67452301;
		unsigned int b = 0xefcdab89;
		unsigned int c = 0x98badcfe;
		unsigned int d = 0x10325476;

		unsigned int SQRT_2 = 0x5a827999;
		unsigned int SQRT_3 = 0x6ed9eba1;
	 
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
	 
		output[0] = a + (unsigned int)0x67452301;
		output[1] = b + (unsigned int)0xefcdab89;
		output[2] = c + (unsigned int)0x98badcfe;
		output[3] = d + (unsigned int)0x10325476;
	}

	void gpu_ntlm_crypt(unsigned int** input, unsigned int** output, int size)
	{
		unsigned int SQRT_2 = 0x5a827999;
		unsigned int SQRT_3 = 0x6ed9eba1;

		unsigned int* aArray = new unsigned int[size];
		unsigned int* bArray = new unsigned int[size];
		unsigned int* cArray = new unsigned int[size];
		unsigned int* dArray = new unsigned int[size];

		for(int i = 0; i < size; i++)
		{
			aArray[i] = 0x67452301;
			bArray[i] = 0xefcdab89;
			cArray[i] = 0x98badcfe;
			dArray[i] = 0x10325476;
		}

		streamOperations a(size); a.writeStream(aArray);
		streamOperations b(size); b.writeStream(bArray);
		streamOperations c(size); c.writeStream(cArray);
		streamOperations d(size); d.writeStream(dArray);

		streamOperations** nt_buffer = new streamOperations*[16];

		for(int i = 0; i < 16; i++)
		{
			nt_buffer[i] = new streamOperations(size);
			nt_buffer[i]->writeStream(input[i]);
		}

		a += (d ^ (b & (c ^ d)))  +  *nt_buffer[0]  ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  *nt_buffer[1]  ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  *nt_buffer[2]  ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  *nt_buffer[3]  ;b = (b << 19) | (b >> 13);
	 
		a += (d ^ (b & (c ^ d)))  +  *nt_buffer[4]  ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  *nt_buffer[5]  ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  *nt_buffer[6]  ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  *nt_buffer[7]  ;b = (b << 19) | (b >> 13);
	 
		a += (d ^ (b & (c ^ d)))  +  *nt_buffer[8]  ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  *nt_buffer[9]  ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  *nt_buffer[10] ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  *nt_buffer[11] ;b = (b << 19) | (b >> 13);
	 
		a += (d ^ (b & (c ^ d)))  +  *nt_buffer[12] ;a = (a << 3 ) | (a >> 29);
		d += (c ^ (a & (b ^ c)))  +  *nt_buffer[13] ;d = (d << 7 ) | (d >> 25);
		c += (b ^ (d & (a ^ b)))  +  *nt_buffer[14] ;c = (c << 11) | (c >> 21);
		b += (a ^ (c & (d ^ a)))  +  *nt_buffer[15] ;b = (b << 19) | (b >> 13);
	 

		a += ((b & (c | d)) | (c & d)) + *nt_buffer[0] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + *nt_buffer[4] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + *nt_buffer[8] +SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + *nt_buffer[12]+SQRT_2; b = (b<<13) | (b>>19);
	 
		a += ((b & (c | d)) | (c & d)) + *nt_buffer[1] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + *nt_buffer[5] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + *nt_buffer[9] +SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + *nt_buffer[13]+SQRT_2; b = (b<<13) | (b>>19);
	 
		a += ((b & (c | d)) | (c & d)) + *nt_buffer[2] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + *nt_buffer[6] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + *nt_buffer[10]+SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + *nt_buffer[14]+SQRT_2; b = (b<<13) | (b>>19);
	 
		a += ((b & (c | d)) | (c & d)) + *nt_buffer[3] +SQRT_2; a = (a<<3 ) | (a>>29);
		d += ((a & (b | c)) | (b & c)) + *nt_buffer[7] +SQRT_2; d = (d<<5 ) | (d>>27);
		c += ((d & (a | b)) | (a & b)) + *nt_buffer[11]+SQRT_2; c = (c<<9 ) | (c>>23);
		b += ((c & (d | a)) | (d & a)) + *nt_buffer[15]+SQRT_2; b = (b<<13) | (b>>19);
	 

		a += (d ^ c ^ b) + *nt_buffer[0]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + *nt_buffer[8]  +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + *nt_buffer[4]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + *nt_buffer[12] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		a += (d ^ c ^ b) + *nt_buffer[2]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + *nt_buffer[10] +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + *nt_buffer[6]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + *nt_buffer[14] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		a += (d ^ c ^ b) + *nt_buffer[1]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + *nt_buffer[9]  +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + *nt_buffer[5]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + *nt_buffer[13] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		a += (d ^ c ^ b) + *nt_buffer[3]  +  SQRT_3; a = (a << 3 ) | (a >> 29);
		d += (c ^ b ^ a) + *nt_buffer[11] +  SQRT_3; d = (d << 9 ) | (d >> 23);
		c += (b ^ a ^ d) + *nt_buffer[7]  +  SQRT_3; c = (c << 11) | (c >> 21);
		b += (a ^ d ^ c) + *nt_buffer[15] +  SQRT_3; b = (b << 15) | (b >> 17);
	 
		a += 0x67452301; a.readStream(output[0]);
		b += 0xefcdab89; b.readStream(output[1]);
		c += 0x98badcfe; c.readStream(output[2]);
		d += 0x10325476; d.readStream(output[3]);

//		delete [] *nt_buffer;
//		delete [] nt_buffer;

//		delete [] dArray;
//		delete [] cArray;
//		delete [] bArray;
//		delete [] aArray;
	}

	char* convert_hex(unsigned int* output)
	{
		//Iterate the integer
		for(int i = 0;i < 4; i++)
		{
			unsigned int n = output[i];
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

	unsigned int *nt_buffer[16];
	unsigned int crypted[4];
	char hex_format[33];
	char* itoa16;

	bool gpu;
	int chunkSize;

	unsigned int** gpu_nt_buffer;
	unsigned int** cpu_nt_buffer;
	unsigned int** gpu_crypted;
	unsigned int** cpu_crypted;
};

#endif
