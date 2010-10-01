/* Slightly modified for LB, orginal code written by Alain Espinosa <alainesp@gmail.com>. Thanks, Alain! */

#ifndef MD4_H_
#define MD4_H_

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cuda.h>

#define itoa16 "0123456789abcdef"

#define SQRT_2 0x5a827999
#define SQRT_3 0x6ed9eba1

#define threadsPerBlock 128
#define blocksPerGrid 1024

__global__ void cuda_encrypt(unsigned int* wd, unsigned int* message)
{
	int i;

	unsigned int tidx = threadIdx.x;
	unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;

	__shared__ unsigned int s_data[(threadsPerBlock * 4) + (threadsPerBlock * 16)];

	unsigned int* s_wd = s_data;
	unsigned int* s_message = s_wd + threadsPerBlock * 4;

	for(i = 0; i < 4; i++)
		s_wd[tidx + i] = wd[idx + i];


	for(i = 0; i < 16; i++)
		s_message[tidx + i] = message[idx + i];

	/* Here, we create aliases to the four 32-bit words used during the encryption process.
	"*a", "*b", "*c", and "*d" are a little easier to read than s_wd[tidx + X]... 		*/
	unsigned int *a = &s_wd[tidx + 0], *b = &s_wd[tidx + 1], *c = &s_wd[tidx + 2], *d = &s_wd[tidx + 3];

	/* Round 1 */
	*a += (*d ^ (*b & (*c ^ *d)))  +  s_message[tidx + 0]  ;*a = (*a << 3 ) | (*a >> 29);
	*d += (*c ^ (*a & (*b ^ *c)))  +  s_message[tidx + 1]  ;*d = (*d << 7 ) | (*d >> 25);
	*c += (*b ^ (*d & (*a ^ *b)))  +  s_message[tidx + 2]  ;*c = (*c << 11) | (*c >> 21);
	*b += (*a ^ (*c & (*d ^ *a)))  +  s_message[tidx + 3]  ;*b = (*b << 19) | (*b >> 13);

	*a += (*d ^ (*b & (*c ^ *d)))  +  s_message[tidx + 0]  ;*a = (*a << 3 ) | (*a >> 29);
	*d += (*c ^ (*a & (*b ^ *c)))  +  s_message[tidx + 1]  ;*d = (*d << 7 ) | (*d >> 25);
	*c += (*b ^ (*d & (*a ^ *b)))  +  s_message[tidx + 2]  ;*c = (*c << 11) | (*c >> 21);
	*b += (*a ^ (*c & (*d ^ *a)))  +  s_message[tidx + 3]  ;*b = (*b << 19) | (*b >> 13);

	*a += (*d ^ (*b & (*c ^ *d)))  +  s_message[tidx + 8]  ;*a = (*a << 3 ) | (*a >> 29);
	*d += (*c ^ (*a & (*b ^ *c)))  +  s_message[tidx + 9]  ;*d = (*d << 7 ) | (*d >> 25);
	*c += (*b ^ (*d & (*a ^ *b)))  +  s_message[tidx + 10] ;*c = (*c << 11) | (*c >> 21);
	*b += (*a ^ (*c & (*d ^ *a)))  +  s_message[tidx + 11] ;*b = (*b << 19) | (*b >> 13);

	*a += (*d ^ (*b & (*c ^ *d)))  +  s_message[tidx + 12] ;*a = (*a << 3 ) | (*a >> 29);
	*d += (*c ^ (*a & (*b ^ *c)))  +  s_message[tidx + 13] ;*d = (*d << 7 ) | (*d >> 25);
	*c += (*b ^ (*d & (*a ^ *b)))  +  s_message[tidx + 14] ;*c = (*c << 11) | (*c >> 21);
	*b += (*a ^ (*c & (*d ^ *a)))  +  s_message[tidx + 15] ;*b = (*b << 19) | (*b >> 13);

	/* Round 2 */
	*a += ((*b & (*c | *d)) | (*c & *d)) + s_message[tidx + 0] +SQRT_2; *a = (*a<<3 ) | (*a>>29);
	*d += ((*a & (*b | *c)) | (*b & *c)) + s_message[tidx + 4] +SQRT_2; *d = (*d<<5 ) | (*d>>27);
	*c += ((*d & (*a | *b)) | (*a & *b)) + s_message[tidx + 8] +SQRT_2; *c = (*c<<9 ) | (*c>>23);
	*b += ((*c & (*d | *a)) | (*d & *a)) + s_message[tidx + 12]+SQRT_2; *b = (*b<<13) | (*b>>19);

	*a += ((*b & (*c | *d)) | (*c & *d)) + s_message[tidx + 1] +SQRT_2; *a = (*a<<3 ) | (*a>>29);
	*d += ((*a & (*b | *c)) | (*b & *c)) + s_message[tidx + 5] +SQRT_2; *d = (*d<<5 ) | (*d>>27);
	*c += ((*d & (*a | *b)) | (*a & *b)) + s_message[tidx + 9] +SQRT_2; *c = (*c<<9 ) | (*c>>23);
	*b += ((*c & (*d | *a)) | (*d & *a)) + s_message[tidx + 13]+SQRT_2; *b = (*b<<13) | (*b>>19);

	*a += ((*b & (*c | *d)) | (*c & *d)) + s_message[tidx + 2] +SQRT_2; *a = (*a<<3 ) | (*a>>29);
	*d += ((*a & (*b | *c)) | (*b & *c)) + s_message[tidx + 6] +SQRT_2; *d = (*d<<5 ) | (*d>>27);
	*c += ((*d & (*a | *b)) | (*a & *b)) + s_message[tidx + 10]+SQRT_2; *c = (*c<<9 ) | (*c>>23);
	*b += ((*c & (*d | *a)) | (*d & *a)) + s_message[tidx + 14]+SQRT_2; *b = (*b<<13) | (*b>>19);

	*a += ((*b & (*c | *d)) | (*c & *d)) + s_message[tidx + 3] +SQRT_2; *a = (*a<<3 ) | (*a>>29);
	*d += ((*a & (*b | *c)) | (*b & *c)) + s_message[tidx + 7] +SQRT_2; *d = (*d<<5 ) | (*d>>27);
	*c += ((*d & (*a | *b)) | (*a & *b)) + s_message[tidx + 11]+SQRT_2; *c = (*c<<9 ) | (*c>>23);
	*b += ((*c & (*d | *a)) | (*d & *a)) + s_message[tidx + 15]+SQRT_2; *b = (*b<<13) | (*b>>19);

	/* Round 3 */
	*a += (*d ^ *c ^ *b) + s_message[tidx + 0]  +  SQRT_3; *a = (*a << 3 ) | (*a >> 29);
	*d += (*c ^ *b ^ *a) + s_message[tidx + 8]  +  SQRT_3; *d = (*d << 9 ) | (*d >> 23);
	*c += (*b ^ *a ^ *d) + s_message[tidx + 4]  +  SQRT_3; *c = (*c << 11) | (*c >> 21);
	*b += (*a ^ *d ^ *c) + s_message[tidx + 12] +  SQRT_3; *b = (*b << 15) | (*b >> 17);

	*a += (*d ^ *c ^ *b) + s_message[tidx + 2]  +  SQRT_3; *a = (*a << 3 ) | (*a >> 29);
	*d += (*c ^ *b ^ *a) + s_message[tidx + 10] +  SQRT_3; *d = (*d << 9 ) | (*d >> 23);
	*c += (*b ^ *a ^ *d) + s_message[tidx + 6]  +  SQRT_3; *c = (*c << 11) | (*c >> 21);
	*b += (*a ^ *d ^ *c) + s_message[tidx + 14] +  SQRT_3; *b = (*b << 15) | (*b >> 17);

	*a += (*d ^ *c ^ *b) + s_message[tidx + 1]  +  SQRT_3; *a = (*a << 3 ) | (*a >> 29);
	*d += (*c ^ *b ^ *a) + s_message[tidx + 9]  +  SQRT_3; *d = (*d << 9 ) | (*d >> 23);
	*c += (*b ^ *a ^ *d) + s_message[tidx + 5]  +  SQRT_3; *c = (*c << 11) | (*c >> 21);
	*b += (*a ^ *d ^ *c) + s_message[tidx + 13] +  SQRT_3; *b = (*b << 15) | (*b >> 17);

	*a += (*d ^ *c ^ *b) + s_message[tidx + 3]  +  SQRT_3; *a = (*a << 3 ) | (*a >> 29);
	*d += (*c ^ *b ^ *a) + s_message[tidx + 11] +  SQRT_3; *d = (*d << 9 ) | (*d >> 23);
	*c += (*b ^ *a ^ *d) + s_message[tidx + 7]  +  SQRT_3; *c = (*c << 11) | (*c >> 21);
	*b += (*a ^ *d ^ *c) + s_message[tidx + 15] +  SQRT_3; *b = (*b << 15) | (*b >> 17);

	// All done. Copy from shared memory back to global.
	for(i = 0; i < 4; i++)
		wd[idx + i] = s_wd[tidx + i];
}

typedef struct ctx
{
		unsigned int wd[4];

		union
		{
			unsigned int uint32[16];
			unsigned char uint8[64];
		} message;

} hashContext;

class MD4
{
public:

	MD4()
	{
		wd_init[0] = 0x67452301;
		wd_init[1] = 0xefcdab89;
		wd_init[2] = 0x98badcfe;
		wd_init[3] = 0x10325476;
	}

	~MD4()
	{
	}

	typedef void (MD4::*scalarHashPtr)(hashContext*);

	inline void getHashContext(hashContext* ctx, unsigned int n)
	{
		wd_h = (unsigned*)malloc(sizeof(unsigned) * 4 * n);
		message_h = (unsigned*)malloc(sizeof(unsigned) * 16 * n);

		cudaMalloc(&wd_d, sizeof(unsigned) * 4 * n);
		cudaMalloc(&message_d, sizeof(unsigned) * 16 * n);

		initialize(ctx, n);

		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < 4; j++)
				wd_h[i*4+j] = ctx[i].wd[j];

			for(int j = 0; j < 16; j++)
				message_h[i*4+j] = ctx[i].message.uint32[j];
		}

		cudaMemcpy(wd_d, wd_h, sizeof(unsigned) * 4 * n, cudaMemcpyHostToDevice);
		cudaMemcpy(message_d, message_d, sizeof(unsigned) * 16 * n, cudaMemcpyHostToDevice);

		cuda_encrypt<<<threadsPerBlock, blocksPerGrid>>>(wd_d, message_d);

		cudaMemcpy(wd_h, wd_d, sizeof(unsigned) * 4 * n, cudaMemcpyDeviceToHost);

		for(int i = 0; i < n; i++)
			for(int j = 0; j < 4; j++)
				ctx[i].wd[j] = wd_h[i*4+j];
	}

protected:

	inline virtual void initialize(hashContext* ctx, int n)
	{
		for(int i = 0; i < n; i++) memcpy(ctx->wd, wd_init, 4 * sizeof(unsigned));
	}

	unsigned int wd_init[4];

	unsigned int* wd_h;
	unsigned int* message_h;

	unsigned int* wd_d;
	unsigned int* message_d;
};

#endif

