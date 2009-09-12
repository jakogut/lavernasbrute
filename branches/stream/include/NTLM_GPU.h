//Part of Laverna's Brute

#ifndef NTLM_GPU_H_
#define NTLM_GPU_H_

//Stream wrapper
#include "StreamOperations.h"

class NTLM_GPU
{
public:

	NTLM_GPU(int size)
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
			cpu_crypted[i] = new unsigned int[4];
			cpu_nt_buffer[i] = new unsigned int[16];
		}

		aArray = new unsigned int[size];
		bArray = new unsigned int[size];
		cArray = new unsigned int[size];
		dArray = new unsigned int[size];

		nt_buffer = new streamOperations*[16];
	}

	~NTLM_GPU()
	{
		delete [] *cpu_crypted;
		delete [] *gpu_crypted;
		delete [] *cpu_nt_buffer;
		delete [] *gpu_nt_buffer;

		delete [] cpu_crypted;
		delete [] gpu_crypted;
		delete [] cpu_nt_buffer;
		delete [] gpu_nt_buffer;

		delete [] *nt_buffer;
		delete [] nt_buffer;

		delete [] dArray;
		delete [] cArray;
		delete [] bArray;
		delete [] aArray;
	}

	//Will be removed once the OpenCL path is finished.
	void getMultipleNTLMHashes(std::string* input, std::string* output)
	{
		prepare_key(input, gpu_nt_buffer);

		gpu_ntlm_crypt(gpu_nt_buffer, gpu_crypted);
	
		for(int i = 0; i < chunkSize; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				cpu_crypted[i][j] = gpu_crypted[j][i];
			}

			output[i] = convert_hex(cpu_crypted[i]);
		}
	}

protected:

	void prepare_key(std::string* inputString, unsigned int** output)
	{
		for(int i = 0; i < chunkSize; i++)
		{
			int j=0;
			int length=(int)(inputString[i].length());

			char* input = new char[length + 1];
			strcpy(input, (char*)inputString[i].c_str());

			memset(cpu_nt_buffer[i],0,16*4);
			//The length of input need to be <= 27
			for(;j<length/2;j++)	
				cpu_nt_buffer[i][j] = input[2*j] | (input[2*j+1]<<16);
		 
			//padding
			if(length%2==1)
				cpu_nt_buffer[i][j] = input[length-1] | 0x800000;
			else
				cpu_nt_buffer[i][j]=0x80;
			//put the length
			cpu_nt_buffer[i][14] = length << 4;

			delete [] input;
		}

		for(int i = 0; i < chunkSize; i++)
			for(int j = 0; j < 16; j++)
				output[j][i] = cpu_nt_buffer[i][j];
	}

	void gpu_ntlm_crypt(unsigned int** input, unsigned int** output)
	{
		unsigned int SQRT_2 = 0x5a827999;
		unsigned int SQRT_3 = 0x6ed9eba1;

		for(int i = 0; i < chunkSize; i++)
		{
			aArray[i] = 0x67452301;
			bArray[i] = 0xefcdab89;
			cArray[i] = 0x98badcfe;
			dArray[i] = 0x10325476;
		}

		streamOperations a(chunkSize); a.writeStream(aArray);
		streamOperations b(chunkSize); b.writeStream(bArray);
		streamOperations c(chunkSize); c.writeStream(cArray);
		streamOperations d(chunkSize); d.writeStream(dArray);

		for(int i = 0; i < 16; i++)
		{
			nt_buffer[i] = new streamOperations(chunkSize);
			nt_buffer[i]->writeStream(gpu_nt_buffer[i]);
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
	 
		a += 0x67452301; a.readStream(output[0]); //a.cleanUp();
		b += 0xefcdab89; b.readStream(output[1]); //b.cleanUp();
		c += 0x98badcfe; c.readStream(output[2]); //c.cleanUp();
		d += 0x10325476; d.readStream(output[3]); //d.cleanUp();

		for(int i = 0; i < 16; i++)
		{
			nt_buffer[i]->cleanUp();
			delete nt_buffer[i];
		}
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

	int chunkSize;

	unsigned int** gpu_nt_buffer;
	unsigned int** cpu_nt_buffer;
	unsigned int** gpu_crypted;
	unsigned int** cpu_crypted;

	unsigned int* aArray;
	unsigned int* bArray;
	unsigned int* cArray;
	unsigned int* dArray;

	streamOperations** nt_buffer;

	char hex_format[33];
	char* itoa16;
};

#endif