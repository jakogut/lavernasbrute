// PArt of Laverna's Brute

#ifndef MD4_CUDA_KERNEL_CUH_
#define MD4_CUDA_KERNEL_CUH_

#include "MD4_CUDA.cuh"

#define F(X, Y, Z) ((X & Y) | (~X & Z))
#define G(X, Y, Z) ((X & Y) | (X & Z) | (Y & Z))
#define H(X, Y, Z) (X ^ Y ^ Z)

#define ROTL(NUM, PLACES, SIZE) ((NUM << PLACES) | (NUM >> (SIZE - PLACES)))
#define ROTR(NUM, PLACES, SIZE) ((NUM >> PLACES) | (NUM << (SIZE - PLACES)))

#define wd(N) (wd[idx * 4 + N])
#define message(N) (message[idx * 16 + N])

__global__ void cuda_init(unsigned int* wd)
{
	unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;

	wd[idx * 4 + 0] = 0x67452301;
	wd[idx * 4 + 1] = 0xefcdab89;
	wd[idx * 4 + 2] = 0x98badcfe;
	wd[idx * 4 + 3] = 0x10325476;
}

__global__ void cuda_encrypt(unsigned int* wd, unsigned int* message)
{
	unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;

	wd(0) += F(wd(1), wd(2), wd(3))  +  message(0),  wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += F(wd(0), wd(1), wd(2))  +  message(1),  wd(3) = ROTL(wd(3), 7, 32);
	wd(2) += F(wd(3), wd(0), wd(1))  +  message(2),  wd(2) = ROTL(wd(2), 11, 32);
	wd(1) += F(wd(2), wd(3), wd(0))  +  message(3),  wd(1) = ROTL(wd(1), 19, 32);

	wd(0) += F(wd(1), wd(2), wd(3))  +  message(4),  wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += F(wd(0), wd(1), wd(2))  +  message(5),  wd(3) = ROTL(wd(3), 7, 32);
	wd(2) += F(wd(3), wd(0), wd(1))  +  message(6),  wd(2) = ROTL(wd(2), 11, 32);
	wd(1) += F(wd(2), wd(3), wd(0))  +  message(7),  wd(1) = ROTL(wd(1), 19, 32);

	wd(0) += F(wd(1), wd(2), wd(3))  +  message(8),  wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += F(wd(0), wd(1), wd(2))  +  message(9),  wd(3) = ROTL(wd(3), 7, 32);
	wd(2) += F(wd(3), wd(0), wd(1))  +  message(10), wd(2) = ROTL(wd(2), 11, 32);
	wd(1) += F(wd(2), wd(3), wd(0))  +  message(11), wd(1) = ROTL(wd(1), 19, 32);

	wd(0) += F(wd(1), wd(2), wd(3))  +  message(12), wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += F(wd(0), wd(1), wd(2))  +  message(13), wd(3) = ROTL(wd(3), 7, 32);
	wd(2) += F(wd(3), wd(0), wd(1))  +  message(14), wd(2) = ROTL(wd(2), 11, 32);
	wd(1) += F(wd(2), wd(3), wd(0))  +  message(15), wd(1) = ROTL(wd(1), 19, 32);

	// Round 2 // ---

	wd(0) += G(wd(1), wd(2), wd(3)) + message(0) +SQRT_2, wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += G(wd(0), wd(1), wd(2)) + message(4) +SQRT_2, wd(3) = ROTL(wd(3), 5, 32);
	wd(2) += G(wd(3), wd(0), wd(1)) + message(8) +SQRT_2, wd(2) = ROTL(wd(2), 9, 32);
	wd(1) += G(wd(2), wd(3), wd(0)) + message(12)+SQRT_2, wd(1) = ROTL(wd(1), 13, 32);

	wd(0) += G(wd(1), wd(2), wd(3)) + message(1) +SQRT_2, wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += G(wd(0), wd(1), wd(2)) + message(5) +SQRT_2, wd(3) = ROTL(wd(3), 5, 32);
	wd(2) += G(wd(3), wd(0), wd(1)) + message(9) +SQRT_2, wd(2) = ROTL(wd(2), 9, 32);
	wd(1) += G(wd(2), wd(3), wd(0)) + message(13)+SQRT_2, wd(1) = ROTL(wd(1), 13, 32);

	wd(0) += G(wd(1), wd(2), wd(3)) + message(2) +SQRT_2, wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += G(wd(0), wd(1), wd(2)) + message(6) +SQRT_2, wd(3) = ROTL(wd(3), 5, 32);
	wd(2) += G(wd(3), wd(0), wd(1)) + message(10)+SQRT_2, wd(2) = ROTL(wd(2), 9, 32);
	wd(1) += G(wd(2), wd(3), wd(0)) + message(14)+SQRT_2, wd(1) = ROTL(wd(1), 13, 32);

	wd(0) += G(wd(1), wd(2), wd(3)) + message(3) +SQRT_2, wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += G(wd(0), wd(1), wd(2)) + message(7) +SQRT_2, wd(3) = ROTL(wd(3), 5, 32);
	wd(2) += G(wd(3), wd(0), wd(1)) + message(11)+SQRT_2, wd(2) = ROTL(wd(2), 9, 32);
	wd(1) += G(wd(2), wd(3), wd(0)) + message(15)+SQRT_2, wd(1) = ROTL(wd(1), 13, 32);

	// Round 3 // --

	wd(0) += H(wd(3), wd(2), wd(1)) + message(0)  +  SQRT_3, wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += H(wd(2), wd(1), wd(0)) + message(8)  +  SQRT_3, wd(3) = ROTL(wd(3), 9, 32);
	wd(2) += H(wd(1), wd(0), wd(3)) + message(4)  +  SQRT_3, wd(2) = ROTL(wd(2), 11, 32);
	wd(1) += H(wd(0), wd(3), wd(2)) + message(12) +  SQRT_3, wd(1) = ROTL(wd(1), 15, 32);

	wd(0) += H(wd(3), wd(2), wd(1)) + message(2)  +  SQRT_3, wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += H(wd(2), wd(1), wd(0)) + message(10) +  SQRT_3, wd(3) = ROTL(wd(3), 9, 32);
	wd(2) += H(wd(1), wd(0), wd(3)) + message(6)  +  SQRT_3, wd(2) = ROTL(wd(2), 11, 32);
	wd(1) += H(wd(0), wd(3), wd(2)) + message(14) +  SQRT_3, wd(1) = ROTL(wd(1), 15, 32);

	wd(0) += H(wd(3), wd(2), wd(1)) + message(1)  +  SQRT_3, wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += H(wd(2), wd(1), wd(0)) + message(9)  +  SQRT_3, wd(3) = ROTL(wd(3), 9, 32);
	wd(2) += H(wd(1), wd(0), wd(3)) + message(5)  +  SQRT_3, wd(2) = ROTL(wd(2), 11, 32);
	wd(1) += H(wd(0), wd(3), wd(2)) + message(13) +  SQRT_3, wd(1) = ROTL(wd(1), 15, 32);

	wd(0) += H(wd(3), wd(2), wd(1)) + message(3)  +  SQRT_3, wd(0) = ROTL(wd(0), 3, 32);
	wd(3) += H(wd(2), wd(1), wd(0)) + message(11) +  SQRT_3, wd(3) = ROTL(wd(3), 9, 32);
	wd(2) += H(wd(1), wd(0), wd(3)) + message(7)  +  SQRT_3, wd(2) = ROTL(wd(2), 11, 32);
	wd(1) += H(wd(0), wd(3), wd(2)) + message(15) +  SQRT_3, wd(1) = ROTL(wd(1), 15, 32);
}

#endif
