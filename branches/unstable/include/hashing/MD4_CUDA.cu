// Part of Laverna's Brute

#include "MD4_CUDA.cuh"
#include "MD4_CUDA_kernel.cu"

MD4_CUDA::MD4_CUDA()
{
}

MD4_CUDA::~MD4_CUDA()
{
}

void MD4_CUDA::getHashContext(hashContext* ctx, unsigned int n)
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
	cudaMemcpy(message_d, message_h, sizeof(unsigned) * 16 * n, cudaMemcpyHostToDevice);

	cuda_encrypt<<<threadsPerBlock, blocksPerGrid>>>(wd_d, message_d);

	cudaMemcpy(wd_h, wd_d, sizeof(unsigned) * 4 * n, cudaMemcpyDeviceToHost);

	for(int i = 0; i < n; i++)
		for(int j = 0; j < 4; j++)
			ctx[i].wd[j] = wd_h[i*4+j];

	cudaFree(message_d);
	cudaFree(wd_d);

	free(message_h);
	free(wd_h);
}

void MD4_CUDA::initialize(hashContext* ctx, int n)
{
	for(int i = 0; i < n; i++) memcpy(ctx[i].wd, wd_init, sizeof(unsigned) * 4);
}
